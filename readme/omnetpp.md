# Configuration File Overview for Blockchain P2P Network Simulation

## Overview

It defines the network's composition, node behaviors, and simulation controls, ensuring that the simulated environment matches the intended experimental conditions.

## Key Parameters

### Network Setup

- `network = Net`: Specifies the network model to be used for the simulation, where `Net` should be defined in your NED (Network Description) files.

### Peer Node Configuration

- `*.peer[*].hashPower = uniform(0.1, 1.0)`: Sets the hash power for each peer node to a random value between 0.1 and 1.0. Hash power affects the rate at which a node can mine blocks.
- `*.peer[*].interarrivalTime = 600`: Defines the average time between block arrivals (or mining attempts) for each peer in seconds. Here, it is set to 600 seconds (10 minutes).
- `*.peer[*].isAdversary = false`: Initially sets all peers as non-adversarial.
- `*.peer[0].isAdversary = true`, `*.peer[1].isAdversary = true`, `*.peer[2].isAdversary = true`: Specifies certain peers as adversaries, which might engage in behaviors like flooding the network with invalid blocks.

### Adversary Configuration

- `*.peer[*].floodingRate = 1`: Sets the rate at which adversarial nodes flood the network with invalid blocks, with one invalid block generated per second on average.

### Simulation Controls

- `sim-time-limit = 86400s`: Limits the simulation run time to 86400 seconds (24 hours), after which the simulation will automatically stop.
- `output-vector-file` and `output-scalar-file`: Configure the paths and names of the output files where the simulation results will be stored. These include vector data (like time series data) and scalar data (like summary statistics).

## Usage

To run the simulation with this configuration:
1. Ensure that the configuration file is placed in the same directory as your OMNeT++ simulation executable or correctly referenced.
2. Open a terminal or command prompt in the directory.
3. Run the simulation using the OMNeT++ command, e.g., `opp_run -u Cmdenv -f omnetpp.ini -c MyConfig`, where `MyConfig` is the configuration name if you have multiple scenarios defined within the same ini file.