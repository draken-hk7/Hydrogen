# Hydrogen Electrolyzer System

A structured engineering package for a low-pressure, separated-gas alkaline water electrolyzer intended for educational prototyping and review.

> **Safety notice:** Hydrogen, oxygen, hot potassium hydroxide, and mains electricity are serious hazards. This repository is not a turnkey construction approval. Mains wiring, pressure hardware, gas storage, ventilation, and KOH handling should be reviewed by qualified professionals before energizing or operating the system.

## Design Target

| Parameter | Target |
|---|---:|
| Input power | 417-833 W continuous |
| Daily energy | 10-20 kWh/day |
| Nominal operating point | ~625 W, ~15 kWh/day |
| Input supply | 230 VAC single phase |
| Electrolyzer type | Alkaline electrolysis, KOH electrolyte |
| Gas streams | Separate H2 and O2, no mixed HHO |
| Stack | 30 cells in series |
| Cell voltage | 1.8-2.2 V practical |
| Stack voltage | ~54-66 V DC |
| Operating temperature | 60-70 C normal, shutdown near 80 C |
| Pressure | Near-atmospheric prototype, relief <=0.3 bar gauge |

## Repository Layout

```text
docs/
  01-executive-summary.md
  02-type-comparison.md
  03-cell-stack-calculations.md
  04-power-electronics.md
  05-gas-safety-architecture.md
  06-electrolyte-management.md
  07-enclosure-plumbing.md
  08-build-steps.md
  09-risks-limitations.md
hardware/
  bom.csv
firmware/
  esp32_electrolyzer_controller/
    esp32_electrolyzer_controller.ino
```

## Quick Links

- [Executive summary](docs/01-executive-summary.md)
- [AEL vs PEM comparison](docs/02-type-comparison.md)
- [Cell stack calculations](docs/03-cell-stack-calculations.md)
- [Power electronics](docs/04-power-electronics.md)
- [Gas safety architecture](docs/05-gas-safety-architecture.md)
- [Electrolyte management](docs/06-electrolyte-management.md)
- [Enclosure and plumbing](docs/07-enclosure-plumbing.md)
- [Build steps](docs/08-build-steps.md)
- [Risks and limitations](docs/09-risks-limitations.md)
- [Complete BOM CSV](hardware/bom.csv)
- [ESP32 control firmware](firmware/esp32_electrolyzer_controller/esp32_electrolyzer_controller.ino)

## Operating Philosophy

The design is intentionally conservative:

- low-pressure operation,
- separated gas paths,
- isolated DC power,
- automatic shutdown on pressure, level, temperature, leak, or emergency-stop faults,
- no hydrogen compression or storage without professional review.

## Professional Review Required

Before building or operating, obtain review for:

- 230 VAC input protection, earthing, enclosure bonding, and DC isolation,
- gas relief valves, flashback arrestor, and any storage cylinder interface,
- ventilation and hydrogen detection placement,
- KOH mixing, containment, spill response, and PPE,
- separator selection and gas crossover risk.
