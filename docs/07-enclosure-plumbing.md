# Enclosure and Plumbing

## Materials

| Part | Recommended material | Notes |
|---|---|---|
| Electrolyte reservoir | HDPE or polypropylene | KOH-compatible |
| Cell frames | HDPE or polypropylene | Machineable and alkaline-resistant |
| Clear viewing parts | Avoid acrylic for hot KOH pressure service | Acrylic may craze/crack |
| Tubing | PTFE, polypropylene, or polyethylene | Check temperature and KOH compatibility |
| Gaskets | EPDM, PTFE, or FKM | Verify compatibility with hot KOH |
| Fasteners | Stainless steel outside electrolyte path | Avoid wetted galvanic couples |
| Electrical enclosure | Earthed metal or flame-retardant plastic | Keep separate from gas compartment |

## Pressure Rating

Use fittings and tubing rated well above expected pressure.

```text
Minimum component rating: >=2 bar
Operating prototype pressure: near atmospheric
Relief target: <=0.3 bar gauge
```

The high component rating is a safety margin, not permission to operate at high pressure.

## Plumbing Philosophy

- separate H2 and O2 from the stack onward,
- no shared bubbler,
- no shared gas reservoir,
- route lines upward where possible to avoid liquid pooling,
- place drains at low points,
- use secondary containment under electrolyte vessels,
- keep gas lines away from electrical terminals.

## P&ID

```mermaid
flowchart LR
  RES[Electrolyte reservoir] --> PUMP[KOH-compatible pump]
  PUMP --> STACK[AEL stack]
  STACK --> HGLS[H2 gas-liquid separator]
  STACK --> OGLS[O2 gas-liquid separator]
  HGLS --> RES
  OGLS --> RES

  HGLS --> HWT[H2 water trap]
  HWT --> HD[Desiccant dryer]
  HD --> HFA[Flashback arrestor]
  HFA --> HV[Outdoor H2 vent or certified storage]

  OGLS --> OWT[O2 water trap]
  OWT --> OV[Outdoor O2 vent]

  HGLS --> HPRV[H2 PRV]
  OGLS --> OPRV[O2 PRV]

  PT1[H2 pressure sensor] --> CTRL[ESP32 controller]
  PT2[O2 pressure sensor] --> CTRL
  TT[Temperature sensor] --> CTRL
  LS[Low level switch] --> CTRL
  H2S[H2 leak detector] --> CTRL
  CTRL --> K1[DC contactor]
  CTRL --> FAN[Fan/pump relay]
  CTRL --> ALARM[Alarm]
```

## Layout Guidance

Keep the electrical compartment, wet electrolyte compartment, and gas handling compartment physically separated. If they must share one enclosure, use internal partitions, drip shields, strain relief, and high-point ventilation.

Recommended placement:

| Item | Placement |
|---|---|
| Stack | Inside secondary containment |
| Gas-liquid separators | Above stack outlet level |
| Reservoir | Below separators for return flow |
| ESP32 | Dry electrical compartment |
| SMPS | Ventilated electrical compartment |
| H2 detector | High point of gas compartment |
| Emergency stop | External, obvious, accessible |
