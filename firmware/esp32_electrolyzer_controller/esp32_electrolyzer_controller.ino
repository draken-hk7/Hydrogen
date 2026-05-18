/*
  ESP32 Electrolyzer Safety Controller

  Target system:
  - Low-pressure alkaline electrolyzer
  - 30-cell stack, nominal 45-66 V DC
  - Separate H2 and O2 gas paths
  - DC contactor drops stack power on any critical fault

  Safety note:
  This firmware is not a certified safety system. Use independent hardware
  interlocks for overpressure, overtemperature, and emergency stop wherever
  possible. Test all shutdown paths before connecting the electrolyzer stack.
*/

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <WebServer.h>

// ------------------------- User configuration -------------------------

#define ENABLE_WIFI_DASHBOARD 1

const char *WIFI_SSID = "CHANGE_ME";
const char *WIFI_PASSWORD = "CHANGE_ME";

// Stack model used for gas production estimate.
const float FARADAY_C_PER_MOL = 96485.0f;
const float FARADAIC_EFFICIENCY = 0.90f;
const int CELL_COUNT = 30;

// Electrical trip limits.
const float STACK_VOLTAGE_MAX_V = 66.0f;
const float STACK_VOLTAGE_MIN_RUNNING_V = 35.0f;
const float STACK_CURRENT_MAX_A = 15.0f;
const float STACK_POWER_MAX_W = 900.0f;

// Pressure limits. Keep DIY prototype near atmospheric pressure.
const float PRESSURE_WARN_BAR = 0.15f;
const float PRESSURE_TRIP_BAR = 0.20f;

// Temperature limits.
const float TEMP_FAN_ON_C = 60.0f;
const float TEMP_WARN_C = 70.0f;
const float TEMP_TRIP_C = 78.0f;

// Sensor fault limits.
const float SENSOR_MIN_VALID_V = 0.05f;
const float SENSOR_MAX_VALID_V = 3.25f;

// ADC calibration. Update these after real calibration.
const float ADC_REF_V = 3.30f;
const int ADC_MAX_COUNTS = 4095;

// Voltage divider: stack+ -> R_TOP -> ADC -> R_BOTTOM -> stack-.
// Example: 330k top and 15k bottom gives ratio 23.0.
const float STACK_VOLTAGE_DIVIDER_RATIO = 23.0f;

// ACS758 style current sensor calibration. Update to match exact module.
// Many Hall sensors are centered around Vcc/2 at zero current.
const float CURRENT_SENSOR_ZERO_V = 1.65f;
const float CURRENT_SENSOR_V_PER_A = 0.020f; // example 20 mV/A

// 0-1 bar, 0.5-4.5 V industrial sensor through divider or direct 3.3 V module.
// If using 0.5-4.5 V sensors, scale externally to ESP32-safe voltage first.
const float PRESSURE_SENSOR_ZERO_V = 0.50f;
const float PRESSURE_SENSOR_FULL_SCALE_V = 3.00f;
const float PRESSURE_SENSOR_FULL_SCALE_BAR = 1.00f;

// ------------------------------ Pin map -------------------------------

const int PIN_STACK_VOLTAGE_ADC = 34;
const int PIN_STACK_CURRENT_ADC = 35;
const int PIN_H2_PRESSURE_ADC = 32;
const int PIN_O2_PRESSURE_ADC = 33;
const int PIN_ONEWIRE_TEMP = 4;

const int PIN_LOW_WATER_SWITCH = 25; // active LOW
const int PIN_H2_LEAK_ALARM = 26;    // active LOW from detector relay
const int PIN_ESTOP = 27;            // active LOW, NC emergency stop chain

const int PIN_CONTACTOR = 18;        // HIGH energizes contactor driver
const int PIN_COOLING = 19;          // HIGH turns pump/fan on
const int PIN_ALARM = 21;            // HIGH turns buzzer/beacon on
const int PIN_PSU_ENABLE = 23;       // optional HIGH enables PSU/PWM driver

// ---------------------------- Global state ----------------------------

OneWire oneWire(PIN_ONEWIRE_TEMP);
DallasTemperature tempSensors(&oneWire);
WebServer server(80);

struct Measurements {
  float stackVoltageV = 0.0f;
  float stackCurrentA = 0.0f;
  float stackPowerW = 0.0f;
  float h2PressureBar = 0.0f;
  float o2PressureBar = 0.0f;
  float temperatureC = 0.0f;
  bool lowWater = false;
  bool h2Leak = false;
  bool eStop = false;
};

Measurements m;

bool contactorEnabled = false;
bool latchedFault = true; // Start safe. User must clear after boot checks.
String faultText = "BOOT: waiting for manual reset";

unsigned long lastSampleMs = 0;
unsigned long lastEnergyMs = 0;
float energyWh = 0.0f;

// -------------------------- Utility functions -------------------------

float adcToVolts(int pin) {
  int raw = analogRead(pin);
  return ((float)raw * ADC_REF_V) / (float)ADC_MAX_COUNTS;
}

float readStackVoltage() {
  return adcToVolts(PIN_STACK_VOLTAGE_ADC) * STACK_VOLTAGE_DIVIDER_RATIO;
}

float readStackCurrent() {
  float v = adcToVolts(PIN_STACK_CURRENT_ADC);
  float amps = (v - CURRENT_SENSOR_ZERO_V) / CURRENT_SENSOR_V_PER_A;
  if (amps < 0.0f) amps = 0.0f;
  return amps;
}

float pressureFromSensorVoltage(float v) {
  float span = PRESSURE_SENSOR_FULL_SCALE_V - PRESSURE_SENSOR_ZERO_V;
  float bar = (v - PRESSURE_SENSOR_ZERO_V) * PRESSURE_SENSOR_FULL_SCALE_BAR / span;
  if (bar < 0.0f) bar = 0.0f;
  return bar;
}

float readPressureBar(int pin) {
  float v = adcToVolts(pin);
  return pressureFromSensorVoltage(v);
}

bool analogSensorLooksValid(int pin) {
  float v = adcToVolts(pin);
  return v >= SENSOR_MIN_VALID_V && v <= SENSOR_MAX_VALID_V;
}

void setOutputsSafe() {
  digitalWrite(PIN_CONTACTOR, LOW);
  digitalWrite(PIN_PSU_ENABLE, LOW);
  contactorEnabled = false;
}

void setAlarm(bool on) {
  digitalWrite(PIN_ALARM, on ? HIGH : LOW);
}

void latchFault(const String &reason) {
  latchedFault = true;
  faultText = reason;
  setOutputsSafe();
  setAlarm(true);
}

void clearFaultIfSafe() {
  // Manual reset is intentionally conservative. In production, use a physical
  // reset button and require all fault inputs to be healthy before clearing.
  if (!m.lowWater && !m.h2Leak && !m.eStop &&
      m.temperatureC < TEMP_WARN_C &&
      m.h2PressureBar < PRESSURE_WARN_BAR &&
      m.o2PressureBar < PRESSURE_WARN_BAR) {
    latchedFault = false;
    faultText = "OK";
    setAlarm(false);
  }
}

// ----------------------- Electrochemistry estimate --------------------

float estimateH2NLPerHour(float currentA) {
  // mol/s = eta * cells * I / (2F)
  float molPerSecond = FARADAIC_EFFICIENCY * (float)CELL_COUNT * currentA / (2.0f * FARADAY_C_PER_MOL);
  // 1 mol ideal gas ~= 22.414 normal liters.
  return molPerSecond * 22.414f * 3600.0f;
}

float estimateO2NLPerHour(float currentA) {
  return estimateH2NLPerHour(currentA) * 0.5f;
}

// ---------------------------- Measurements ----------------------------

void readMeasurements() {
  m.stackVoltageV = readStackVoltage();
  m.stackCurrentA = readStackCurrent();
  m.stackPowerW = m.stackVoltageV * m.stackCurrentA;
  m.h2PressureBar = readPressureBar(PIN_H2_PRESSURE_ADC);
  m.o2PressureBar = readPressureBar(PIN_O2_PRESSURE_ADC);

  tempSensors.requestTemperatures();
  m.temperatureC = tempSensors.getTempCByIndex(0);
  if (m.temperatureC < -50.0f || m.temperatureC > 125.0f) {
    m.temperatureC = 999.0f; // Force trip on invalid temperature reading.
  }

  m.lowWater = digitalRead(PIN_LOW_WATER_SWITCH) == LOW;
  m.h2Leak = digitalRead(PIN_H2_LEAK_ALARM) == LOW;
  m.eStop = digitalRead(PIN_ESTOP) == LOW;
}

void updateEnergyCounter() {
  unsigned long now = millis();
  if (lastEnergyMs == 0) {
    lastEnergyMs = now;
    return;
  }

  float hours = (float)(now - lastEnergyMs) / 3600000.0f;
  lastEnergyMs = now;

  if (contactorEnabled && !latchedFault) {
    energyWh += m.stackPowerW * hours;
  }
}

// ------------------------------ Faults --------------------------------

void evaluateFaults() {
  if (m.eStop) latchFault("EMERGENCY STOP ACTIVE");
  if (m.lowWater) latchFault("LOW WATER LEVEL");
  if (m.h2Leak) latchFault("HYDROGEN LEAK ALARM");

  if (!analogSensorLooksValid(PIN_H2_PRESSURE_ADC)) latchFault("H2 PRESSURE SENSOR FAULT");
  if (!analogSensorLooksValid(PIN_O2_PRESSURE_ADC)) latchFault("O2 PRESSURE SENSOR FAULT");
  if (!analogSensorLooksValid(PIN_STACK_CURRENT_ADC)) latchFault("CURRENT SENSOR FAULT");

  if (m.temperatureC >= TEMP_TRIP_C) latchFault("OVERTEMPERATURE");
  if (m.h2PressureBar >= PRESSURE_TRIP_BAR) latchFault("H2 OVERPRESSURE");
  if (m.o2PressureBar >= PRESSURE_TRIP_BAR) latchFault("O2 OVERPRESSURE");
  if (m.stackVoltageV > STACK_VOLTAGE_MAX_V) latchFault("STACK OVERVOLTAGE");
  if (m.stackCurrentA > STACK_CURRENT_MAX_A) latchFault("STACK OVERCURRENT");
  if (m.stackPowerW > STACK_POWER_MAX_W) latchFault("STACK OVERPOWER");

  if (contactorEnabled && m.stackCurrentA > 1.0f && m.stackVoltageV < STACK_VOLTAGE_MIN_RUNNING_V) {
    latchFault("LOW STACK VOLTAGE WHILE RUNNING");
  }
}

void updateControls() {
  bool coolingNeeded = m.temperatureC >= TEMP_FAN_ON_C;
  digitalWrite(PIN_COOLING, coolingNeeded ? HIGH : LOW);

  if (latchedFault) {
    setOutputsSafe();
    return;
  }

  digitalWrite(PIN_PSU_ENABLE, HIGH);
  digitalWrite(PIN_CONTACTOR, HIGH);
  contactorEnabled = true;
}

// ---------------------------- Dashboard -------------------------------

String htmlEscape(const String &s) {
  String out;
  for (size_t i = 0; i < s.length(); i++) {
    char c = s[i];
    if (c == '<') out += "&lt;";
    else if (c == '>') out += "&gt;";
    else if (c == '&') out += "&amp;";
    else out += c;
  }
  return out;
}

void handleRoot() {
  float h2 = estimateH2NLPerHour(m.stackCurrentA);
  float o2 = estimateO2NLPerHour(m.stackCurrentA);

  String page = "<!doctype html><html><head><meta name='viewport' content='width=device-width,initial-scale=1'>";
  page += "<meta http-equiv='refresh' content='3'>";
  page += "<title>Electrolyzer Controller</title>";
  page += "<style>body{font-family:Arial;margin:24px;max-width:760px}table{border-collapse:collapse;width:100%}td,th{border:1px solid #ccc;padding:8px}.fault{color:#b00020;font-weight:bold}.ok{color:#096b2e;font-weight:bold}</style>";
  page += "</head><body><h1>Electrolyzer Controller</h1>";
  page += latchedFault ? "<p class='fault'>FAULT: " + htmlEscape(faultText) + "</p>" : "<p class='ok'>Status: OK</p>";
  page += "<table><tr><th>Measurement</th><th>Value</th></tr>";
  page += "<tr><td>Stack voltage</td><td>" + String(m.stackVoltageV, 2) + " V</td></tr>";
  page += "<tr><td>Stack current</td><td>" + String(m.stackCurrentA, 2) + " A</td></tr>";
  page += "<tr><td>Stack power</td><td>" + String(m.stackPowerW, 1) + " W</td></tr>";
  page += "<tr><td>Energy</td><td>" + String(energyWh, 1) + " Wh</td></tr>";
  page += "<tr><td>Temperature</td><td>" + String(m.temperatureC, 1) + " C</td></tr>";
  page += "<tr><td>H2 pressure</td><td>" + String(m.h2PressureBar, 3) + " bar gauge</td></tr>";
  page += "<tr><td>O2 pressure</td><td>" + String(m.o2PressureBar, 3) + " bar gauge</td></tr>";
  page += "<tr><td>Estimated H2</td><td>" + String(h2, 1) + " NL/h</td></tr>";
  page += "<tr><td>Estimated O2</td><td>" + String(o2, 1) + " NL/h</td></tr>";
  page += "<tr><td>Low water</td><td>" + String(m.lowWater ? "YES" : "NO") + "</td></tr>";
  page += "<tr><td>H2 leak alarm</td><td>" + String(m.h2Leak ? "YES" : "NO") + "</td></tr>";
  page += "<tr><td>E-stop</td><td>" + String(m.eStop ? "ACTIVE" : "OK") + "</td></tr>";
  page += "</table>";
  page += "<p><a href='/reset'>Clear fault if safe</a></p>";
  page += "</body></html>";

  server.send(200, "text/html", page);
}

void handleReset() {
  clearFaultIfSafe();
  server.sendHeader("Location", "/");
  server.send(303);
}

void setupWifi() {
#if ENABLE_WIFI_DASHBOARD
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 10000) {
    delay(250);
  }

  server.on("/", handleRoot);
  server.on("/reset", handleReset);
  server.begin();
#endif
}

// ------------------------------- Setup --------------------------------

void setup() {
  Serial.begin(115200);

  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);

  pinMode(PIN_LOW_WATER_SWITCH, INPUT_PULLUP);
  pinMode(PIN_H2_LEAK_ALARM, INPUT_PULLUP);
  pinMode(PIN_ESTOP, INPUT_PULLUP);

  pinMode(PIN_CONTACTOR, OUTPUT);
  pinMode(PIN_COOLING, OUTPUT);
  pinMode(PIN_ALARM, OUTPUT);
  pinMode(PIN_PSU_ENABLE, OUTPUT);

  setOutputsSafe();
  setAlarm(true);

  tempSensors.begin();
  setupWifi();

  Serial.println("Electrolyzer controller booted. Fault latched until reset.");
}

// -------------------------------- Loop ---------------------------------

void loop() {
#if ENABLE_WIFI_DASHBOARD
  server.handleClient();
#endif

  unsigned long now = millis();
  if (now - lastSampleMs >= 1000) {
    lastSampleMs = now;

    readMeasurements();
    updateEnergyCounter();
    evaluateFaults();
    updateControls();

    Serial.print("V="); Serial.print(m.stackVoltageV, 2);
    Serial.print(" I="); Serial.print(m.stackCurrentA, 2);
    Serial.print(" P="); Serial.print(m.stackPowerW, 1);
    Serial.print(" T="); Serial.print(m.temperatureC, 1);
    Serial.print(" H2bar="); Serial.print(m.h2PressureBar, 3);
    Serial.print(" O2bar="); Serial.print(m.o2PressureBar, 3);
    Serial.print(" H2NLh="); Serial.print(estimateH2NLPerHour(m.stackCurrentA), 1);
    Serial.print(" Wh="); Serial.print(energyWh, 1);
    Serial.print(" Fault="); Serial.println(latchedFault ? faultText : "OK");
  }
}
