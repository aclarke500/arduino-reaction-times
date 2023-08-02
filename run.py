import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

df = pd.read_csv('adam-sober.csv')
all_values = df['reaction'].values.tolist()

plt.hist(all_values, bins=20, density=True, alpha=0.6, color='blue')

# Fit a bell curve to the all_values
mean = np.mean(all_values)
std = np.std(all_values)
x = np.linspace(min(all_values), max(all_values), 100)
y = (1/(std * np.sqrt(2 * np.pi))) * np.exp(-0.5 * ((x - mean) / std)**2)

# Plot the bell curve on top of the histogram
plt.plot(x, y, color='red', linewidth=2)


# Set plot labels and title
plt.xlabel('Value')
plt.ylabel('Frequency')
plt.title('Distribution of Reaction Times')
plt.show()

