#ifndef MYMESSAGE_H
#define MYMESSAGE_H

#include <omnetpp.h>
#include <string>
#include <vector>
#include <ctime>

using namespace omnetpp;
using namespace std;

class MyMessage : public cMessage {
public:
    std::string messageType;  // Type of the message (e.g., "register", "peerList", "block", "blockRequest")
    std::string content;      // Main content of the message
    int address;              // Address of the peer/seed
    set<int> peerlist;        // List of peers
    simtime_t timestamp;      // Time at which the message was sent
    int sender_address;       // Address of the sender

    std::string blockHash;    // Hash of the block (for block messages)
    std::string prevHash;     // Previous block hash
    std::string merkleRoot;   // Merkle root of the block
    time_t blockTimestamp;    // Timestamp of the block
};

#endif // MYMESSAGE_H
