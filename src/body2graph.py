import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import argparse
import io

def plot_tendency_from_file(filename, tendency_curve=0.1):
    # Read file and stop at the first empty line
    with open(filename, 'r') as file:
        lines = []
        for line in file:
            if line.strip():  # Only add non-empty lines
                lines.append(line)
            else:
                break  # Stop reading at the first empty line
    
    # Convert to DataFrame
    df = pd.read_csv(io.StringIO("\n".join(lines)), sep=r'\s+', header=None, names=['Date', 'Weight', 'Exercises'], parse_dates=['Date'])
    
    # Sort data by date
    df = df.sort_values(by='Date')
    
    # Treat 0 as NaN and forward fill with the last valid value for Weight
    df['Weight'] = df['Weight'].replace(0, np.nan).ffill().infer_objects(copy=False)
    
    # Fill missing Exercises values with 0
    df['Exercises'] = df['Exercises'].fillna(0)
    
    # Initialize tendency for weight with NaN
    weight_tendency = np.nan
    weight_tendency_values = []
    
    # Start calculating tendency only after the first valid weight
    for i in range(len(df)):
        if pd.isna(df['Weight'].iloc[i]):
            # If the weight is NaN, append the previous weight tendency value
            weight_tendency_values.append(weight_tendency)
        else:
            # Calculate weight tendency using the formula
            if pd.isna(weight_tendency):
                weight_tendency = df['Weight'].iloc[i]
            else:
                weight_tendency = weight_tendency + tendency_curve * (df['Weight'].iloc[i] - weight_tendency)
            
            weight_tendency_values.append(weight_tendency)
    
    # Add the calculated weight tendency values to the dataframe
    df['Weight_Tendency'] = weight_tendency_values
    
    # Group exercises by month (sum of exercises in each month)
    df_monthly_exercises = df.resample('ME', on='Date')['Exercises'].sum()

    # Plot weight tendency with no data points for weight and exercises
    fig, ax1 = plt.subplots(figsize=(10, 6))
    
    # Plot weight tendency curve
    ax1.plot(df['Date'], df['Weight_Tendency'], 'r--', label='Weight Tendency')
    
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
    parser.add_argument('filename', nargs='?', default='private/body.txt', type=str, help='Path to the data file (default: private/body.txt)')
    parser.add_argument('--tendency_curve', type=float, default=0.1, help='Tendency curve factor for weight (default: 0.1)')
    args = parser.parse_args()
    
    plot_tendency_from_file(args.filename, tendency_curve=args.tendency_curve)

