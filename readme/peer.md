# PeerNode Module for Blockchain P2P Network Simulation

## Overview

The `PeerNode` class, defined within the `peer.cc` file, is an integral component of a blockchain-based peer-to-peer (P2P) network simulation using OMNeT++. This module simulates the functionalities of a peer node in the blockchain network, including blockchain operations, network interactions, and the mining process.

## Key Features

- **Blockchain Operations**: Manages a local blockchain where blocks are added following validation.
- **Network Communication**: Handles network interactions such as registering with seed nodes, requesting peer lists, connecting to peers, and broadcasting blocks.
- **Mining Simulation**: Simulates the mining of new blocks using a proof-of-work (PoW) algorithm and schedules mining operations based on node-specific hash power.
- **Adversary Simulation**: Optionally acts as an adversary within the network, simulating attacks by flooding the network with invalid blocks.
- **Liveness Checks**: Periodically checks the connectivity and responsiveness of connected peers to maintain network integrity.
- **Gossip Protocol**: Implements a basic gossip protocol to disseminate information across the network.

## Methods

### Initialization and Message Handling
- `initialize()`: Sets up the node, including loading parameters, creating the genesis block, and initiating network connections.
- `handleMessage(cMessage *msg)`: Routes incoming messages based on type (e.g., block, peer list, liveness test) and handles them accordingly.

### Blockchain and Network Functions
- `mineBlock()`: Attempts to mine a new block based on the PoW concept.
- `validateAndAddBlock(Block block)`: Validates and adds a new block to the blockchain.
- `broadcastBlock(const Block& block)`: Broadcasts a successfully mined block to connected peers.
- `syncBlockchain()`: Synchronizes the blockchain with peers to ensure consistency across the network.

### Utility Functions
- `calculateHash(const Block& block)`: Calculates the hash for a block.
- `sha256(const string& str)`: Generates a SHA-256 hash of a given string, truncated to fit the blockchain's requirements.
- `logBlockchain()`: Logs the current state of the blockchain to a file.

### Cleanup
- `finish()`: Finalizes the node's state at the end of the simulation, logging summary statistics.

## Usage in Simulation

This module is designed to operate within a larger OMNeT++ simulation framework. It interacts with other node modules (such as `SeedNode`) to simulate a full blockchain network environment. The functionality of `PeerNode` can be configured through parameters in the simulation's `.ini` file, allowing adjustments to node behavior, hash power, and adversary roles.