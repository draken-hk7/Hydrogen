# Build Steps

These steps are written for an intermediate builder, but the system still combines mains electricity, hydrogen, oxygen, caustic electrolyte, and hot liquid. Stop and obtain professional review whenever the build deviates from the design.

## Ordered Procedure

1. Review the design with an electrician and, ideally, a chemist or process engineer.

2. Finalize the low-pressure design. Do not include compression or storage in the first build.

3. Source safety-critical components first: RCD/ELCB, isolated PSU, DC fuse, DC contactor, relief valves, flashback arrestor, check valves, hydrogen detector, PPE.

4. Fabricate or purchase HDPE/PP cell frames for 30 cells.

5. Cut nickel electrodes to provide about 100 cm2 active area per cell. Deburr all edges.

6. Cut separator sheets. Use Zirfon or an alkaline electrolysis diaphragm; do not substitute cloth, paper, or unknown plastic films.

7. Assemble the stack dry using gaskets and spacers. Verify even compression and no electrode shorting.

8. Leak-test the stack with clean water before adding electrolyte.

9. Assemble the H2 and O2 gas-liquid separators, traps, relief valves, and vent lines. Keep the streams separate.

10. Build the electrical panel with MCB/RCD, isolated PSU, DC fuse, contactor, current sensor, and control power supply. Professional review strongly recommended before energizing.

11. Wire the ESP32 controller and verify sensor readings using safe simulated values before connecting the stack.

12. Prepare 25 wt% KOH by slowly adding KOH flakes into DI water while wearing full PPE. Allow the solution to cool.

13. Fill the reservoir and stack. Circulate electrolyte with power off and inspect for leaks.

14. Energize at low current, around 100-200 W, with outdoor ventilation active.

15. Confirm separate H2 and O2 gas evolution, stable pressure readings, and no leaks.

16. Test automatic shutdown functions: emergency stop, low water, high temperature, pressure alarm, H2 leak alarm, and sensor fault.

17. Increase to 417 W mode and run for a short monitored trial.

18. If stable, test 625 W nominal mode.

19. Test 833 W only after thermal behavior and pressure stability are understood.

20. Do not connect hydrogen to storage or fuel use until gas purity and pressure hardware have been professionally reviewed.

## Commissioning Checklist

| Check | Pass criteria |
|---|---|
| Electrical earth | Verified continuity and RCD trip |
| DC isolation | No exposed terminals, fuse installed |
| Stack leak test | No liquid leak at operating temperature |
| Gas separation | No shared H2/O2 headspace |
| Relief valves | Installed on both gas paths |
| Flashback arrestor | Installed on H2 line |
| H2 detector | Trips shutdown path |
| Ventilation | Forced exhaust active |
| Firmware | Trips contactor on every fault |
| PPE | Available near system |

## First Run Limits

```text
Initial current: <=3 A
Initial power: <=200 W
Initial duration: 10-20 minutes
Operator: present at all times
Location: outdoor or forced-ventilated shed
```
