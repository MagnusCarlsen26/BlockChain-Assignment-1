Overviewon# Overview for Blockchain P2P Network Simulation

## Overview

`MyMessage.h` is a header file in an OMNeT++ simulation project, primarily defining a custom message class used across the simulation for blockchain network operations. This file includes the definitions necessary for creating, handling, and manipulating messages exchanged between different nodes (peer and seed) in the simulated blockchain network.

## Key Features

- **Custom Message Class**: Defines the structure of messages used for node communication within the network.
- **Message Types**: Enumerates different types of messages such as block announcements, registration requests, peer list requests, and liveness checks.

## Components

### MyMessage Class

The `MyMessage` class extends the base `cMessage` class provided by OMNeT++. It includes several attributes specific to blockchain operations:
- `messageType`: A string that indicates the type of the message, e.g., "register", "block", "peerList".
- `content`: Contains any data associated with the message, such as block data or peer information.
- `address`: Used to identify the sender or the intended recipient, depending on the message context.
- `prevHash`, `merkleRoot`, `blockTimestamp`, `blockHash`: Attributes used specifically in messages related to blockchain blocks.
- `peerlist`: A collection of peer IDs, used when exchanging peer lists.

### Usage in Simulation

The custom messages defined in `MyMessage.h` facilitate various operations critical to maintaining and operating a blockchain network:
- **Network Formation and Maintenance**: Messages like registration requests and peer list exchanges help in forming and maintaining the network.
- **Blockchain Consistency**: Block-related messages ensure that all nodes in the network maintain a consistent view of the blockchain.
- **Node Liveness**: Liveness checking messages help in monitoring the availability of peers within the network.

## Integration

This header is included in the node implementations (`peer.cc`, `seed.cc`) to utilize the defined message structures for simulation events. Nodes create instances of `MyMessage` to communicate state changes, request data, and synchronize blockchain content with one another.