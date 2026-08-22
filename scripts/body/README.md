# Body Weight Log

A simple, human-friendly body weight log and visualization tool.

## Philosophy

This project is intentionally designed around **manual data capture**.

The primary goal is to make recording daily information as frictionless as possible, even when using paper or a glass window as a temporary buffer.

The persistent text file is considered the canonical source of truth and is optimized for:

- human readability
- long-term stability
- easy editing
- compatibility with Python, AWK, C, shell scripts, and pandas

The file format should remain simple enough that it can still be edited comfortably after many years.

Complexity should only be added when a real use case appears.

---

# File Format

Each line represents one day.

```
YYYY-MM-DD Weight [Token...]
```

Examples:

```
2026-06-19 57.0
2026-06-20 -
2026-06-21 56.8 |
2026-06-22 56.5 X
2026-06-23 56.4 bike
2026-06-24 56.2 bike muscle
2026-06-25 - fast:18
```

Blank lines terminate the input (current behavior).

All word tokens (exercise types and parameterized event names) are case-insensitive.

---

# Weight

The second field represents body weight.

Possible values:

```
57.3
56.8
58.1
```

or

```
-
```

or, in older entries,

```
0
```

Meaning:

- decimal number → measured weight
- `-` → weight not measured on that day
- `0` → weight not measured on that day (legacy notation, superseded by `-`)

Missing weights must be interpreted as `NaN`. `-` is the current notation for a missing weight; `0` is an older, equivalent notation kept only for backward compatibility with existing logs and must be treated identically. New entries should use `-`.

The plotting code should forward-fill previous valid weights before calculating the tendency curve.

---

# Event Tokens

All remaining fields are optional event tokens.

Tokens are independent.

Example:

```
2026-07-10 56.4 bike muscle fast:18
```

means

- bike
- gym
- 18-hour fast

---

## Generic exercise

These tokens exist for backward compatibility.

```
1
|
```

Both mean

```
Generic exercise = 1
```

Likewise

```
2
X
```

Both mean

```
Generic exercise = 2
```

More generally, any bare integer token (`1`, `2`, `3`, ...) is a legacy notation meaning

```
Generic exercise = <that number>
```

`|` and `X` are just aliases for `1` and `2`. This covers older entries that recorded a raw count higher than 2 (e.g. `3`) directly in the token field.

Old log files should continue working without modification.

---

## Exercise Types

Current tokens (case-insensitive):

| Token | Meaning |
|--------|---------|
| bike | Bike |
| muscle | Gym / Strength training |
| walk | Walk |
| run | Run |
| stairs | Stairs |

These events are currently stored but not used for plotting.

Future analyses may use them.

---

## Parameterized Events

Parameterized events consist of a word, a colon, and a value: `word:value`.

Current example:

```
fast:18
```

means

```
18-hour fast
```

The parser should interpret this as

```
FastHours = 18
```

Other current parameterized events:

| Token pattern | Meaning | Example |
|--------|---------|---------|
| fast:\<integer\> | Fasting hours | `fast:18` → FastHours = 18 |
| fat:\<decimal\> | Body fat percentage | `fat:16.4` → FatPercent = 16.4 |
| muscle:\<decimal\> | Skeletal muscle mass (kg) | `muscle:25.4` → MuscleMass = 25.4 |
| water:\<integer\> | Hydration percentage (bioimpedance reference value) | `water:60` → Hydration = 60 |
| force:\<integer\> | Push-up count during a force test | `force:25` → PushUps = 25 |

The `muscle` name is shared with the bare `muscle` exercise token (Gym), disambiguated by the presence of the `:value` suffix: `muscle` alone means Gym, while `muscle:<value>` means muscle mass.

`force` is captured but not yet used by the plotting code.

Additional parameterized events may be added in the future.

---

# Data Model

The parser should return a pandas DataFrame.

Suggested columns:

```
Date
Weight
Exercises
Bike
Gym
Walk
Run
Swim
FastHours
FatPercent
MuscleMass
Hydration
PushUps
WeightTendency
FatPercentTendency
MuscleMassTendency
HydrationTendency
```

`Exercises` is the sum of generic and specific exercises and it is the only currently used by the plotting code.

Additional columns exist to preserve information for future analyses.

---

# Tendency Curve

Weight tendency is calculated using exponential smoothing.

```
tendency = tendency + α * (value - tendency)
```

where

```
α = tendency_curve
```

(default 0.1)

Missing values should not interrupt the tendency calculation: the underlying series is forward-filled before smoothing.

The same smoothing is applied to the bioimpedance metrics (FatPercent, MuscleMass, Hydration), since they are measured far less regularly than weight — forward-filling and smoothing turns sparse, irregular readings into a continuous curve suitable for plotting.

The exercises tendency (plotted in panel 1) is smoothed the same way, but on top of the **monthly sum** of `Exercises` rather than the raw daily count — the daily series is a spiky 0/1/2/... event count, too jagged for exponential smoothing to read as a trend. Smoothing the monthly totals instead produces a curve of "how much am I exercising lately", comparable in shape to the weight tendency curve.

---

# Plot

The generated figure has three stacked panels sharing the date axis:

1. **Weight and exercises tendency** — weight tendency curve (left axis) and exercises tendency curve (right axis), both exponentially smoothed.
2. **Exercises by type** — monthly counts of Bike, Gym, Walk, Run, Swim, stacked as bars.
3. **Bioimpedance** — tendency curves (same exponential smoothing as weight) for FatPercent and Hydration (percentage, left axis), MuscleMass (kg, right axis) and Weight (kg, secondary right axis) for context, rather than raw scatter points, since these are measured too infrequently for scatter to read well.

---

# Temporary Recording

The persistent file is **not** intended to be the primary recording medium.

Daily information is often first written on paper or on a glass window using an intentionally compact notation.

Examples:

```
57.1
56.9 |
56.8 X
56.7 B
56.6 BM
56.5 F18
```

During transcription these marks are converted into the persistent format.

This separation between **capture format** and **storage format** is intentional.

---

# Backward Compatibility

Maintaining compatibility with historical logs is an explicit project goal.

Older entries like

```
2020-05-17 57.4 1
```

must remain valid indefinitely.

Likewise, older entries using `0` as the weight field, e.g.

```
2026-04-10 0 1
```

must continue to be interpreted as a missing weight, same as `-`.

Future extensions should avoid breaking existing files whenever possible.

---

# Parser Design

The parser should **not** expect a fixed number of columns.

Instead:

- first token → date
- second token → weight
- remaining tokens → events

This keeps the format extensible while preserving simplicity.

---

# Project Structure

The project intentionally remains small.

Suggested organization:

```
body.py

parse_body_file()
calculate_weight_tendency()
plot_weight_tendency()
main()
```

Avoid introducing additional modules or abstractions unless they solve a concrete problem.

Simplicity is preferred over flexibility.

---

# Design Principles

When modifying this project, prefer the following priorities:

1. Keep manual recording fast.
2. Preserve backward compatibility.
3. Keep the file human-readable.
4. Avoid unnecessary abstractions.
5. Make future extensions additive rather than disruptive.
6. Keep the parser tolerant of new event tokens.
7. Keep plotting independent from parsing whenever practical.

This project values long-term maintainability over feature richness.
