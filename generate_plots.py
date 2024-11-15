import os
import re
import matplotlib.pyplot as plt
import numpy as np

def parse_log_file(filename):
    with open(filename, 'r') as f:
        content = f.readlines()
    
    plot_data = []
    for line in content:
        if line.startswith('PLOT_DATA'):
            data = line.strip().split(',')
            plot_data.append([float(x) for x in data[1:]])  # Skip the 'PLOT_DATA' prefix
    
    return np.array(plot_data)

def process_all_logs(directory):
    all_peer_data = []
    for filename in os.listdir(directory):
        if filename.startswith('peer_') and filename.endswith('_log.txt'):
            full_path = os.path.join(directory, filename)
            peer_data = parse_log_file(full_path)
            all_peer_data.append(peer_data)
    return all_peer_data

def plot_results(all_peer_data, flood_percentage):
    combined_data = np.concatenate(all_peer_data)
    
    combined_data = combined_data[combined_data[:, 0].argsort()]
    
    sim_time = combined_data[:, 0]
    utilization = combined_data[:, 1]
    adversary_fraction = combined_data[:, 2]
    chain_length = combined_data[:, 3]
    
    avg_utilization = np.mean(utilization.reshape(-1, len(all_peer_data)), axis=1)
    
    plt.figure(figsize=(10, 5))
    plt.plot(sim_time[::len(all_peer_data)], avg_utilization)
    plt.xlabel('Simulation Time (s)')
    plt.ylabel('Average Mining Power Utilization')
    plt.title(f'Average Mining Power Utilization vs Time ({flood_percentage}% Flooded)')
    plt.savefig(f'avg_mining_power_utilization_{flood_percentage}.png')
    plt.close()

    plt.figure(figsize=(10, 5))
    plt.plot(sim_time, adversary_fraction)
    plt.xlabel('Simulation Time (s)')
    plt.ylabel('Fraction of Main Chain Blocks by Adversary')
    plt.title(f'Adversary Fraction vs Time ({flood_percentage}% Flooded)')
    plt.savefig(f'adversary_fraction_{flood_percentage}.png')
    plt.close()
    
    plt.figure(figsize=(10, 5))
    plt.plot(sim_time, chain_length)
    plt.xlabel('Simulation Time (s)')
    plt.ylabel('Main Chain Length')
    plt.title(f'Main Chain Length vs Time ({flood_percentage}% Flooded)')
    plt.savefig(f'main_chain_length_{flood_percentage}.png')
    plt.close()

def plot_combined_results(flood_percentages):
    plt.figure(figsize=(12, 6))
    for percentage in flood_percentages:
        directory = f"flood_{percentage}_lenient"
        all_peer_data = process_all_logs(directory)
        combined_data = np.concatenate(all_peer_data)
        combined_data = combined_data[combined_data[:, 0].argsort()]
        
        sim_time = combined_data[:, 0]
        utilization = combined_data[:, 1]
        avg_utilization = np.mean(utilization.reshape(-1, len(all_peer_data)), axis=1)
        
        plt.plot(sim_time[::len(all_peer_data)], avg_utilization, label=f'{percentage}% Flooded')
    
    plt.xlabel('Simulation Time (s)')
    plt.ylabel('Average Mining Power Utilization')
    plt.title('Average Mining Power Utilization vs Time (All Scenarios)')
    plt.legend()
    plt.savefig('combined_avg_mining_power_utilization.png')
    plt.close()

if __name__ == "__main__":
    flood_percentages = [10, 20, 30]
    
    for percentage in flood_percentages:
        directory = f"./flood_{percentage}_leniant"
        all_peer_data = process_all_logs(directory)
        plot_results(all_peer_data, percentage)
    
    plot_combined_results(flood_percentages)
    print("Plots have been saved for all flooding scenarios.")