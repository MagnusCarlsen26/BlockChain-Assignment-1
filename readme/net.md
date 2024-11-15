### 1. **SeedNode Definition**
This module represents a seed node in the blockchain network. Seed nodes facilitate the initial connection for peer nodes and help in distributing network information.

- **Parameters**:
  - `ns`: Total number of seed nodes in the network.
  - `np`: Total number of peer nodes that can connect to this seed node.
  
- **Gates**:
  - `input in[np]`: Input gates for connections from peer nodes.
  - `output out[np]`: Output gates for sending information to peer nodes.

### 2. **PeerNode Definition**
This module simulates the functionalities of a peer node in the blockchain network. Peer nodes are responsible for mining blocks, maintaining the blockchain, and communicating with other nodes.

- **Parameters**:
  - `ns`: Number of seed nodes available for connection.
  - `np`: Number of peer nodes in the network.
  - `adv`: Index of a peer node designated as an adversary.
  - `hashPower`: Computational power of the node used for mining blocks, defined by a uniform distribution between 0.1 and 1.0.
  - `interarrivalTime`: Average time in seconds between block arrivals or mining events, set to 600 seconds (10 minutes).
  - `isAdversary`: Boolean flag indicating whether the node acts as an adversary.
  - `floodingRate`: Rate at which an adversarial node floods the network with invalid blocks.

- **Gates**:
  - `input in[ns]` and `output out[ns]`: Connect to seed nodes.
  - `input peerin[np]` and `output peerout[np]`: Connect to other peer nodes.

### 3. **Network Configuration (Net)**
Defines the entire network setup including the number of seed and peer nodes, and the connections between them.

- **Parameters**:
  - `numSeeds`: Total number of seed nodes in the network.
  - `numPeers`: Total number of peer nodes in the network.
  - `adv`: Randomly selects one peer to act as an adversary using a uniform distribution.

- **Submodules**:
  - `seed[numSeeds]`: Creates multiple instances of `SeedNode` based on `numSeeds`.
  - `peer[numPeers]`: Creates multiple instances of `PeerNode` based on `numPeers`.

- **Connections**:
  - Commented out in your code snippet, typically would establish connections between peers and seeds. Dynamic peer-to-peer connections are noted to be established during the simulation.