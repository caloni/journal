import numpy as np
import matplotlib
matplotlib.use('QtAgg')  # or 'TkAgg'
import matplotlib.pyplot as plt
import matplotlib.dates as mdates
import pandas as pd
import argparse
import re


EXERCISE_TOKEN_ALIASES = {
    '|': 1,
    'x': 2,
}


def generic_exercise_weight(token):
    if token.lower() in EXERCISE_TOKEN_ALIASES:
        return EXERCISE_TOKEN_ALIASES[token.lower()]
    if token.isdigit():
        return int(token)
    return 0

SPECIFIC_EXERCISE_COLUMNS = {
    'bike': 'Bike',
    'muscle': 'Muscle',
    'walk': 'Walk',
    'run': 'Run',
    'stairs': 'Stairs',
}

PARAMETERIZED_EVENT_COLUMNS = {
    'fat': ('FatPercent', float),
    'fast': ('FastHours', int),
    'muscle': ('MuscleMass', float),
    'water': ('Hydration', float),
    'force': ('PushUps', int),
}

PARAMETERIZED_EVENT_PATTERN = re.compile(r'([A-Za-z0-9]+):(\d+(?:\.\d+)?)')


def parse_body_file(filename):
    rows = []

    with open(filename, 'r', encoding='utf-8') as file:
        for raw_line in file:
            line = raw_line.strip()
            if not line:
                break

            tokens = line.split()
            if len(tokens) < 2:
                continue

            date_token = tokens[0]
            weight_token = tokens[1]
            event_tokens = tokens[2:]

            weight = np.nan if weight_token == '-' else pd.to_numeric(weight_token, errors='coerce')
            if weight == 0:
                # Legacy convention: '0' also marks a missing weight measurement.
                weight = np.nan

            row = {
                'Date': pd.to_datetime(date_token, errors='coerce'),
                'Weight': weight,
                'Bike': 0,
                'Muscle': 0,
                'Walk': 0,
                'Run': 0,
                'Stairs': 0,
                'FastHours': np.nan,
                'FatPercent': np.nan,
                'MuscleMass': np.nan,
                'Hydration': np.nan,
                'PushUps': np.nan,
            }

            generic_exercises = 0

            for token in event_tokens:
                token_lower = token.lower()

                if token_lower in SPECIFIC_EXERCISE_COLUMNS:
                    row[SPECIFIC_EXERCISE_COLUMNS[token_lower]] += 1
                    continue

                param_match = PARAMETERIZED_EVENT_PATTERN.fullmatch(token)
                if param_match:
                    name = param_match.group(1).lower()
                    if name in PARAMETERIZED_EVENT_COLUMNS:
                        column, cast = PARAMETERIZED_EVENT_COLUMNS[name]
                        row[column] = cast(param_match.group(2))
                        continue

                generic_exercises += generic_exercise_weight(token)

            row['Exercises'] = generic_exercises + row['Bike'] + row['Muscle'] + row['Walk'] + row['Run'] + row['Stairs']
            rows.append(row)

    df = pd.DataFrame(
        rows,
        columns=['Date', 'Weight', 'Exercises', 'Bike', 'Muscle', 'Walk', 'Run', 'Stairs',
                 'FastHours', 'FatPercent', 'MuscleMass', 'Hydration', 'PushUps']
    )

    if df.empty:
        return df

    df = df.dropna(subset=['Date'])
    df = df.sort_values(by='Date').reset_index(drop=True)
    return df


def exponential_tendency(series, tendency_curve):
    filled = series.ffill()
    tendency = np.nan
    values = []

    for value in filled:
        if pd.isna(value):
            values.append(np.nan)
            continue

        if pd.isna(tendency):
            tendency = value
        else:
            tendency = tendency + tendency_curve * (value - tendency)

        values.append(tendency)

    return values


TENDENCY_COLUMNS = {
    'Weight': 'WeightTendency',
    'FatPercent': 'FatPercentTendency',
    'MuscleMass': 'MuscleMassTendency',
    'Hydration': 'HydrationTendency',
}


def calculate_weight_tendency(df, tendency_curve=0.1):
    if df.empty:
        for tendency_column in TENDENCY_COLUMNS.values():
            df[tendency_column] = pd.Series(dtype=float)
        return df

    for source_column, tendency_column in TENDENCY_COLUMNS.items():
        df[tendency_column] = exponential_tendency(df[source_column], tendency_curve)

    return df

EXERCISE_TYPE_COLORS = {
    'Bike': 'tab:blue',
    'Muscle': 'saddlebrown',
    'Walk': 'tab:green',
    'Stairs': 'gold',
    'Run': 'tab:red',
}

def plot_tendency_from_file(filename, tendency_curve=0.1):
    df = parse_body_file(filename)
    df = calculate_weight_tendency(df, tendency_curve=tendency_curve)

    if df.empty:
        raise ValueError('No valid data rows found in the input file.')

    exercise_types = list(SPECIFIC_EXERCISE_COLUMNS.values())
    df_monthly_exercise_types = df.resample('ME', on='Date')[exercise_types].sum()

    df_monthly_exercises = df.resample('ME', on='Date')['Exercises'].sum()
    monthly_exercises_tendency = exponential_tendency(df_monthly_exercises, tendency_curve)

    fig, (ax_weight, ax_exercise_types, ax_bio) = plt.subplots(3, 1, figsize=(10, 12), sharex=True)

    # Panel 1: weight tendency + exercises tendency
    ax_weight.plot(df['Date'], df['WeightTendency'], 'r--', label='Weight Tendency')
    ax_weight.set_ylabel('Weight Tendency', color='red')
    ax_weight.tick_params(axis='y', labelcolor='red')

    ax_exercises_total = ax_weight.twinx()
    ax_exercises_total.plot(df_monthly_exercises.index, monthly_exercises_tendency, color='green', label='Exercises Tendency')
    ax_exercises_total.set_ylabel('Exercises Tendency', color='green')
    ax_exercises_total.tick_params(axis='y', labelcolor='green')

    ax_weight.set_title('Weight Tendency and Exercises Tendency')
    weight_handles, weight_labels = ax_weight.get_legend_handles_labels()
    exercises_handles, exercises_labels = ax_exercises_total.get_legend_handles_labels()
    ax_exercises_total.legend(weight_handles + exercises_handles, weight_labels + exercises_labels,
                               loc='upper left', bbox_to_anchor=(1.08, 1))
    ax_weight.grid()

    # Panel 2: monthly exercises broken down by type (stacked bars)
    bottom = np.zeros(len(df_monthly_exercise_types))
    for exercise_type in exercise_types:
        values = df_monthly_exercise_types[exercise_type].to_numpy()
        ax_exercise_types.bar(df_monthly_exercise_types.index, values, bottom=bottom,
                               label=exercise_type, color=EXERCISE_TYPE_COLORS[exercise_type], width=20)
        bottom += values

    ax_exercise_types.set_ylabel('Monthly Count')
    ax_exercise_types.set_title('Monthly Exercises by Type')
    ax_exercise_types.legend(loc='upper left', bbox_to_anchor=(1.08, 1))
    ax_exercise_types.grid()

    # Panel 3: bioimpedance tendency curves (sparse measurements smoothed like weight tendency)
    ax_bio.plot(df['Date'], df['FatPercentTendency'], color='tab:orange', label='Fat % Tendency')
    ax_bio.plot(df['Date'], df['HydrationTendency'], color='tab:blue', label='Hydration % Tendency')
    ax_bio.set_ylabel('Percentage')
    ax_bio.grid()

    ax_muscle = ax_bio.twinx()
    ax_muscle.plot(df['Date'], df['MuscleMassTendency'], color='tab:green', label='Muscle Mass Tendency (kg)')
    ax_muscle.set_ylabel('Muscle Mass (kg)', color='tab:green')
    ax_muscle.tick_params(axis='y', labelcolor='tab:green')

    ax_weight_bio = ax_bio.twinx()
    ax_weight_bio.spines['right'].set_position(('axes', 1.15))
    ax_weight_bio.plot(df['Date'], df['WeightTendency'], 'r--', label='Weight Tendency (kg)')
    ax_weight_bio.set_ylabel('Weight (kg)', color='red')
    ax_weight_bio.tick_params(axis='y', labelcolor='red')

    bio_handles, bio_labels = ax_bio.get_legend_handles_labels()
    muscle_handles, muscle_labels = ax_muscle.get_legend_handles_labels()
    weight_bio_handles, weight_bio_labels = ax_weight_bio.get_legend_handles_labels()
    ax_weight_bio.legend(bio_handles + muscle_handles + weight_bio_handles,
                          bio_labels + muscle_labels + weight_bio_labels,
                          loc='upper left', bbox_to_anchor=(1.25, 1))

    ax_bio.set_title('Bioimpedance Metrics')
    ax_bio.set_xlabel('Date')

    # Twin axes with partially-NaN series can shrink the shared x-range; pin it back to the full data span.
    ax_weight.set_xlim(df['Date'].min(), df['Date'].max())

    locator = mdates.AutoDateLocator(minticks=4, maxticks=8)
    ax_bio.xaxis.set_major_locator(locator)
    ax_bio.xaxis.set_major_formatter(mdates.ConciseDateFormatter(locator))

    plt.xticks(rotation=45)
    fig.tight_layout(rect=[0, 0, 0.8, 1])
    plt.show()

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Plot weight tendency and monthly exercise graphs.')
    parser.add_argument('filename', nargs='?', default='.private/body.txt', type=str, help='Path to the data file (default: .private/body.txt)')
    parser.add_argument('--tendency_curve', type=float, default=0.1, help='Tendency curve factor for weight (default: 0.1)')
    args = parser.parse_args()
    
    plot_tendency_from_file(args.filename, tendency_curve=args.tendency_curve)

