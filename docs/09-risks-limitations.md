# Risks and Limitations

## Main Risks

| Risk | What can happen | Mitigation |
|---|---|---|
| H2/O2 crossover | Explosive mixture in gas line | Real separator, low pressure, gas purity testing |
| Blocked outlet | Overpressure and rupture | Relief valves, pressure sensors, open vent discipline |
| Hydrogen leak | Fire or explosion | Ventilation, H2 detector, ignition control |
| Flashback | Flame travels into gas line | Hydrogen-rated flashback arrestor |
| KOH exposure | Severe chemical burns | PPE, secondary containment, eyewash, training |
| Hot electrolyte leak | Burns and equipment damage | Compatible materials, thermal shutdown |
| Mains shock | Injury or death | RCD/ELCB, earthing, isolated PSU, electrician review |
| DC arc | Fire or burns | DC fuse, contactor, covered terminals |
| Impure hydrogen | Unsafe fuel/storage gas | Analyzer verification before use |
| Separator failure | Gas mixing | Differential pressure control, conservative operation |
| Electrode corrosion | Contamination, failure | Prefer nickel, inspect regularly |

## System Limitations

This design does not certify:

- hydrogen purity >=99.5% without gas analysis,
- suitability for compressed storage,
- indoor operation without site-specific ventilation design,
- food, medical, laboratory, or industrial gas compliance,
- unattended 24/7 operation without redundant independent safety systems.

## Recommended Additional Safeguards

For serious operation, add:

- independent hardwired overtemperature cutoff,
- independent hardwired overpressure cutoff,
- calibrated hydrogen detector with relay output,
- oxygen contamination sensor on H2 outlet,
- data logging with fault history,
- watchdog timer and failsafe contactor logic,
- flameproof or isolated electrical compartment,
- professionally designed outdoor vent stack.

## Red Lines

Do not operate if any of these are true:

- H2 and O2 share a bubbler or headspace,
- relief valves are absent or blocked,
- ventilation is off,
- there is no H2 leak detector,
- KOH leaks are present,
- electrical terminals are exposed,
- firmware shutdown has not been tested,
- hydrogen is routed to improvised storage.
