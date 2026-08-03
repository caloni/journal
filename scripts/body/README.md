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
2026-06-23 56.4 B
2026-06-24 56.2 B M
2026-06-25 - F18
```

Blank lines terminate the input (current behavior).

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
2026-07-10 56.4 B M F18
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

Current tokens:

| Token | Meaning |
|--------|---------|
| B | Bike |
| M | Gym / Strength training |
| C | Walk |
| R | Run |
| S | Swim |

These events are currently stored but not used for plotting.

Future analyses may use them.

---

## Parameterized Events

Parameterized events consist of a letter followed by a value.

Current example:

```
F18
```

means

```
18-hour fast
```

The parser should interpret this as

```
FastHours = 18
```

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
WeightTendency
```

`Exercises` is the sum of generic and specific exercises and it is the only currently used by the plotting code.

Additional columns exist to preserve information for future analyses.

---

# Tendency Curve

Weight tendency is calculated using exponential smoothing.

```
tendency = tendency + α * (weight - tendency)
```

where

```
α = tendency_curve
```

(default 0.1)

Missing weights should not interrupt the tendency calculation.

---

# Monthly Exercise Plot

The existing graph displays

- weight tendency
- monthly sum of Exercises

Only exercises sum contribute to this graph.

Typed exercise events (Bike, Gym, etc.) sum to Exercises field but their independent values are intentionally ignored for now.

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
