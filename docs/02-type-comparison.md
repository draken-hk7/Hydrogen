# Electrolyzer Type Comparison

## AEL vs PEM for This Scale

| Factor | AEL with KOH | PEM |
|---|---|---|
| DIY feasibility | Best practical option, still hazardous | Poor for raw DIY fabrication |
| Electrolyte | 20-30 wt% KOH | DI water, membrane conducts protons |
| Separator | Zirfon or alkaline diaphragm | Nafion or equivalent PEM |
| Typical electrode materials | Nickel, nickel mesh, nickel-coated stainless | Titanium, Ir/Ru/Pt catalyst layers |
| Practical cell voltage | 1.8-2.2 V | 1.7-2.1 V |
| Gas separation | Good with correct diaphragm and low pressure | Excellent with commercial MEA |
| Cost at 417-833 W | Lower | Much higher |
| Maintenance | Electrolyte concentration, carbonation, water top-up | DI water purity, membrane/catalyst health |
| Safety concerns | KOH burns, H2 fire, gas crossover | H2 fire, high pressure, membrane failure |
| Indian sourcing | Feasible with local fabrication | Commercial modules easier than raw materials |
| Recommendation | Recommended design path | Buy certified module only |

## Recommendation

Use alkaline electrolysis for the home-buildable prototype. PEM is attractive technically, but a safe PEM stack requires high-quality membrane electrode assemblies, titanium flow fields, catalyst coatings, compression hardware, and very pure water management. Under a ₹25,000 initial budget, PEM is usually either too small or too compromised.

## Practical PEM Variant

If using PEM, buy a certified low-pressure PEM module rather than building the cell stack from scratch.

Suggested PEM constraints:

| Parameter | PEM variant recommendation |
|---|---:|
| Power | 100-500 W commercial module |
| Water | DI water only, very low conductivity |
| Pressure | Manufacturer-rated only |
| Gas handling | Same H2/O2 separation and flashback controls |
| Controller | Same monitoring approach, adjusted voltage/current limits |
| Budget fit | Usually outside ₹25,000 for 625 W continuous |
