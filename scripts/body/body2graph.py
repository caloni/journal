import numpy as np
import matplotlib
matplotlib.use('QtAgg')  # or 'TkAgg'
import matplotlib.pyplot as plt
import pandas as pd
import argparse
import re


EXERCISE_TOKEN_ALIASES = {
    '|': 1,
    'X': 2,
}


def generic_exercise_weight(token):
    if token in EXERCISE_TOKEN_ALIASES:
        return EXERCISE_TOKEN_ALIASES[token]
    if token.isdigit():
        return int(token)
    return 0

SPECIFIC_EXERCISE_COLUMNS = {
    'B': 'Bike',
    'M': 'Gym',
    'C': 'Walk',
    'R': 'Run',
    'S': 'Swim',
}


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
                'Gym': 0,
                'Walk': 0,
                'Run': 0,
                'Swim': 0,
                'FastHours': np.nan,
            }

            generic_exercises = 0

            for token in event_tokens:
                generic_exercises += generic_exercise_weight(token)

                if token in SPECIFIC_EXERCISE_COLUMNS:
                    row[SPECIFIC_EXERCISE_COLUMNS[token]] += 1
                    continue

                fast_match = re.fullmatch(r'F(\d+)', token)
                if fast_match:
                    row['FastHours'] = int(fast_match.group(1))

            row['Exercises'] = generic_exercises + row['Bike'] + row['Gym'] + row['Walk'] + row['Run'] + row['Swim']
            rows.append(row)

    df = pd.DataFrame(
        rows,
        columns=['Date', 'Weight', 'Exercises', 'Bike', 'Gym', 'Walk', 'Run', 'Swim', 'FastHours']
    )

    if df.empty:
        return df

    df = df.dropna(subset=['Date'])
    df = df.sort_values(by='Date').reset_index(drop=True)
    return df


def calculate_weight_tendency(df, tendency_curve=0.1):
    if df.empty:
        df['WeightTendency'] = pd.Series(dtype=float)
        return df

    weight_for_tendency = df['Weight'].ffill()
    weight_tendency = np.nan
    weight_tendency_values = []

    for weight in weight_for_tendency:
        if pd.isna(weight):
            weight_tendency_values.append(np.nan)
            continue

        if pd.isna(weight_tendency):
            weight_tendency = weight
        else:
            weight_tendency = weight_tendency + tendency_curve * (weight - weight_tendency)

        weight_tendency_values.append(weight_tendency)

    df['WeightTendency'] = weight_tendency_values
    return df

def plot_tendency_from_file(filename, tendency_curve=0.1):
    df = parse_body_file(filename)
    df = calculate_weight_tendency(df, tendency_curve=tendency_curve)

    if df.empty:
        raise ValueError('No valid data rows found in the input file.')
    
    # Group exercises by month (sum of exercises in each month)
    df_monthly_exercises = df.resample('ME', on='Date')['Exercises'].sum()

    # Plot weight tendency with no data points for weight and exercises
    fig, ax1 = plt.subplots(figsize=(10, 6))
    
    # Plot weight tendency curve
    ax1.plot(df['Date'], df['WeightTendency'], 'r--', label='Weight Tendency')
    
    ax1.set_xlabel('Date')
    ax1.set_ylabel('Weight Tendency', color='red')
    ax1.tick_params(axis='y', labelcolor='red')
    
    # Create a second y-axis for exercises data
    ax2 = ax1.twinx()
    
    # Plot exercises data (monthly) with transparent green points
    ax2.scatter(df_monthly_exercises.index, df_monthly_exercises, label='Monthly Exercises', color='green', alpha=0.3)
    
    ax2.set_ylabel('Monthly Exercises', color='green')
    ax2.tick_params(axis='y', labelcolor='green')
    
    plt.title('Weight Tendency and Monthly Exercises')
    fig.tight_layout()  # Adjust layout for better fit
    ax1.legend(loc='upper left')
    ax2.legend(loc='upper right')
    
    plt.grid()
    plt.xticks(rotation=45)
    plt.show()

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Plot weight tendency and monthly exercise graphs.')
    parser.add_argument('filename', nargs='?', default='.private/body.txt', type=str, help='Path to the data file (default: .private/body.txt)')
    parser.add_argument('--tendency_curve', type=float, default=0.1, help='Tendency curve factor for weight (default: 0.1)')
    args = parser.parse_args()
    
    plot_tendency_from_file(args.filename, tendency_curve=args.tendency_curve)

