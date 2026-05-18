# Cell Stack Calculations

## Design Assumptions

| Parameter | Value |
|---|---:|
| Faraday constant, F | 96485 C/mol |
| Faradaic efficiency, eta_F | 90% nominal |
| Cells in series | 30 |
| Practical cell voltage | 2.05 V |
| Stack voltage | 61.5 V |
| Active area per electrode | 100 cm2 |
| Electrolyte | 25 wt% KOH |

## Faraday Law

```text
n = Q / (z * F)
```

For hydrogen:

```text
2H2O + 2e- -> H2 + 2OH-
z = 2 electrons per mole H2
```

For oxygen:

```text
4OH- -> O2 + 2H2O + 4e-
z = 4 electrons per mole O2
```

## Stack Voltage

```text
V_stack = N_cells * V_cell
V_stack = 30 * 2.05 = 61.5 V
```

## Current at Power Levels

```text
I = P / V_stack
```

| Power | Stack voltage | Current |
|---:|---:|---:|
| 417 W | 61.5 V | 6.8 A |
| 625 W | 61.5 V | 10.2 A |
| 833 W | 61.5 V | 13.5 A |

## Hydrogen Production

Because the 30 cells are in series, the same current passes through every cell and each cell produces gas. Therefore:

```text
mol_H2_per_second = eta_F * N_cells * I / (2 * F)
```

At 625 W:

```text
I = 625 / 61.5 = 10.16 A
mol_H2/s = 0.90 * 30 * 10.16 / (2 * 96485)
         = 0.00142 mol/s
mol_H2/day = 0.00142 * 86400
           = 123 mol/day
Nm3/day = 123 * 22.414 / 1000
        = 2.75 Nm3/day
NL/hour = 2.75 * 1000 / 24
        = 115 NL/h
```

Oxygen is produced at half the hydrogen molar rate:

```text
mol_O2 = mol_H2 / 2
```

## Production Table

| Power | Current | H2 NL/h | H2 Nm3/day | H2 kg/day | O2 NL/h | O2 Nm3/day |
|---:|---:|---:|---:|---:|---:|---:|
| 417 W | 6.8 A | ~76 | ~1.83 | ~0.165 | ~38 | ~0.92 |
| 625 W | 10.2 A | ~115 | ~2.75 | ~0.248 | ~57 | ~1.38 |
| 833 W | 13.5 A | ~153 | ~3.66 | ~0.330 | ~76 | ~1.83 |

## Electrode Area

```text
A = I / J
```

At maximum current, 13.5 A:

| Current density | Required active area |
|---:|---:|
| 200 mA/cm2 | 67.5 cm2 |
| 300 mA/cm2 | 45.0 cm2 |
| 400 mA/cm2 | 33.8 cm2 |

Use 100 cm2 active area per cell. This reduces current density, improves thermal margin, and helps gas bubble release.

## Cell Geometry

| Feature | Recommendation |
|---|---|
| Cell count | 30 bipolar cells |
| Active area | 100 mm x 100 mm |
| Electrode gap | 2-4 mm each side of separator |
| Separator | Zirfon PERL or alkaline-rated diaphragm |
| Frame | HDPE or polypropylene |
| Gaskets | EPDM, PTFE, or FKM |
| Flow | Bottom electrolyte inlet, top gas/electrolyte outlet |
| Orientation | Vertical plates |

## Electrode Material Ranking

| Rank | Material | Notes |
|---:|---|---|
| 1 | Nickel 200 mesh or perforated sheet | Best DIY AEL choice |
| 2 | Nickel-plated stainless mesh | Acceptable compromise |
| 3 | 316L stainless steel | Works but corrodes/passivates, especially as anode |
| 4 | Titanium with Ir/Ru coating | Excellent but not budget friendly |
