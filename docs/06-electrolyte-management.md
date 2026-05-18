# Electrolyte Management

## KOH Concentration

Use 25 wt% potassium hydroxide as the design point.

Example for 10 kg electrolyte:

```text
KOH mass = 2.5 kg
Water mass = 7.5 kg
```

Acceptable operating range:

```text
20-30 wt% KOH
```

## Mixing Procedure

Professional chemical safety review is strongly recommended before handling KOH.

Safety rules:

- wear chemical goggles or face shield,
- wear KOH-compatible gloves and apron,
- mix outdoors or under strong ventilation,
- use HDPE or polypropylene containers,
- add KOH flakes slowly to water,
- never add water onto dry KOH,
- allow the solution to cool before filling the system.

KOH dissolution is exothermic and can heat the solution enough to splash or boil if rushed.

## Water Quality

Use distilled or DI water only.

Tap water adds minerals and contaminants such as calcium, magnesium, chloride, iron, and carbonate. These can increase electrode corrosion, reduce efficiency, poison surfaces, form deposits, and increase side reactions.

## Replenishment

Electrolysis consumes water, not KOH. Replenish only with DI/distilled water unless concentration measurement confirms KOH correction is needed.

Initial maintenance schedule:

| Frequency | Action |
|---|---|
| Every run | Check leaks, level, temperature, sensor status |
| Daily | Top up DI water if needed |
| Weekly | Check conductivity and inspect electrolyte color |
| Monthly | Inspect electrodes, gaskets, tubing, traps, dryer |
| As needed | Replace desiccant, clean traps, correct KOH concentration |

## Temperature Management

Target operating range:

```text
Normal: 60-70 C
Caution: 70-75 C
Shutdown: 75-80 C
```

Heat estimate at 625 W and 65% overall useful efficiency:

```text
Heat ~= 625 W * (1 - 0.65)
     ~= 219 W
```

Recommended thermal design:

- allow self-heating at startup,
- use a KOH-compatible circulation loop,
- use a small radiator or heat exchanger isolated from electrical parts,
- switch fan/pump on near 60 C,
- shut down the DC contactor if temperature exceeds the limit.

## Sensors

| Measurement | Low-cost option | Better option |
|---|---|---|
| Temperature | DS18B20 probe outside wetted KOH | K-type thermocouple in compatible thermowell |
| Conductivity | Low-cost EC module | Industrial KOH-compatible conductivity probe |
| Level | Float switch | External capacitive level sensor |
| pH | pH strip for spot check | High-alkalinity-rated pH probe |
| Pressure | 0-1 bar transducer | Certified low-pressure transmitter |
