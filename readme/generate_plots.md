# Plot Generation for Blockchain Network Simulation

## Overview

This Python script, `generate_plots.py`, is designed to parse log files from a blockchain P2P network simulation, aggregate the data, and generate visualizations of key metrics. These include average mining power utilization, the fraction of main chain blocks mined by adversaries, and the main chain length over the course of the simulation. The script supports multiple scenarios characterized by different percentages of node flooding to assess network resilience.

## Features

- **Log File Parsing:** Extracts relevant simulation data tagged with 'PLOT_DATA' from log files.
- **Data Aggregation:** Combines data from multiple peer nodes for comprehensive analysis.
- **Visualization:** Generates plots for individual flooding scenarios and a combined plot comparing all scenarios.

## Usage

### Running the Script

Execute the script from the command line:

```bash
python generate_plots.py
```

### Required Python Libraries

- `matplotlib`: For generating plots.
- `numpy`: For numerical operations and data manipulation.

These can be installed using pip:

```bash
pip install matplotlib numpy
```

### Directory Structure

Ensure that the log files are named in the format `peer_X_log.txt` and are organized in directories named `flood_X_lenient` where `X` is the flooding percentage (e.g., 10, 20, 30). Each directory should correspond to a different scenario of node flooding.

## Output

The script will generate the following plots:
- `avg_mining_power_utilization_X.png`: Shows the average mining power utilization over time for each flooding percentage `X`.
- `adversary_fraction_X.png`: Displays the fraction of main chain blocks mined by the adversary as a function of time for each scenario.
- `main_chain_length_X.png`: Plots the length of the main chain over time for each flooding scenario.
- `combined_avg_mining_power_utilization.png`: A combined plot comparing average mining power utilization across different flooding scenarios.

Each plot is saved in the current directory as a PNG file.