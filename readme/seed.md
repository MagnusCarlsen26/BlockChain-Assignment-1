# SeedNode Module for Blockchain P2P Network Simulation

## Overview

The `SeedNode` module is an integral part of a blockchain-based peer-to-peer (P2P) network simulation implemented in OMNeT++. This module simulates the functionality of seed nodes, which are essential for the formation and maintenance of the network. Seed nodes facilitate the registration of peer nodes, manage a list of active peers, and ensure the removal of non-responsive nodes to maintain network integrity.

## Functionality

### Initialization
- **Hash Calculation:** On initialization, the seed node calculates a hash for demonstration purposes, showcasing how hash functions might be utilized within the network.

### Message Handling
- **Registration:** Handles registration requests from peer nodes. Upon receiving a registration message, the seed node adds the peer's information to its peer list and sends a confirmation back to the peer.
- **Peer List Request:** Responds to peer list requests from registered nodes by sending them a list of currently active peers.
- **Dead Node Reporting:** Removes nodes from the active peer list based on reports from other nodes indicating a peer is no longer responsive.

### Key Methods
- `initialize()`: Sets up initial conditions or demonstrations as the simulation starts.
- `handleMessage(cMessage *msg)`: Deals with incoming messages according to their type (`register`, `peerList`, `Dead Node`), modifying the peer list as appropriate and responding to the peers.
- `to_hex(size_t hash)`: Converts a numerical hash value into a hexadecimal string.
- `cal_hash(std::string str)`: Generates a hash from a string using a standard hash function, illustrating potential use in blockchain operations.

## Usage in Simulation

This module is designed to be part of a larger OMNeT++ simulation framework and interacts with other modules representing different types of nodes within the blockchain network. It utilizes custom message types defined in `MyMessage.h` to facilitate communication between nodes.