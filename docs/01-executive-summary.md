# Executive Summary

This system design describes a low-pressure alkaline water electrolyzer that produces hydrogen and oxygen in separate streams. It is intended for educational prototyping and engineering review, not unsupervised indoor fuel production.

## Key Specifications

| Item | Design target |
|---|---:|
| Electrolyzer type | Alkaline electrolysis, separated H2/O2 |
| Input | 230 VAC single phase, isolated DC conversion |
| Daily energy | 10-20 kWh/day |
| Nominal power | 625 W continuous |
| Stack | 30 cells in series |
| Nominal stack voltage | ~61.5 V DC |
| Nominal current | ~10.2 A at 625 W |
| Electrode active area | 100 cm2 per cell |
| Electrolyte | 25 wt% KOH, 8-12 L total |
| Operating temperature | 60-70 C preferred |
| Shutdown temperature | 75-80 C |
| Prototype pressure | Near-atmospheric |
| Relief setpoint | <=0.3 bar gauge recommended |
| Estimated build cost | ₹22,000-₹35,000 |

## Production Estimate

Assuming 30 cells, 2.05 V/cell, and 90% Faradaic efficiency:

| Power | H2 NL/h | H2 Nm3/day | H2 kg/day | O2 NL/h | O2 Nm3/day |
|---:|---:|---:|---:|---:|---:|
| 417 W | ~76 | ~1.83 | ~0.165 | ~38 | ~0.92 |
| 625 W | ~115 | ~2.75 | ~0.248 | ~57 | ~1.38 |
| 833 W | ~153 | ~3.66 | ~0.330 | ~76 | ~1.83 |

At 70% Faradaic efficiency, multiply the gas outputs by 0.778.

## Recommended Architecture

Use an isolated 60 V class CC/CV DC supply feeding a 30-cell alkaline stack. The gas collection system must use fully separate H2 and O2 paths, separate gas-liquid separators, separate water traps, separate relief valves, and a hydrogen-rated flashback arrestor on the H2 line.

## Strong Professional Review Flags

Professional review is strongly recommended before proceeding with:

- 230 VAC wiring, RCD/ELCB, fusing, earthing, and SMPS/transformer enclosure,
- pressure relief hardware and any gas storage interface,
- hydrogen flashback arrestor selection,
- ventilation and leak detector placement,
- KOH handling and spill response procedure,
- gas purity verification if H2 is used for fuel or storage.
