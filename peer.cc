#include <omnetpp.h>
#include <vector>
#include <algorithm>
#include <numeric>
#include <sstream>
#include <random>
#include "MyMessage.h"
#include <functional>
#include <iomanip>
#include <ctime>
#include <queue>
#include <cstring>
#include <fstream>
#include <cmath>





using namespace omnetpp;
using namespace std;

struct Block {
    string prevHash;
    string merkleRoot;
    time_t timestamp;
    string hash;
    int height;
};



class PeerNode : public cSimpleModule
{
private:
    vector<Block> blockchain;
    queue<Block> pendingQueue;
    string currentHash;
    double nodeHashPower;
    double interarrivalTime;
    cMessage *miningTimer;
    bool isAdversary;
    int floodingRate;
    vector<int> floodTargets;
    int blocksMined;
    int totalBlocksMined;
    cOutVector miningPowerUtilization;
    cOutVector adversaryFraction;
    simtime_t lastRecordTime;
    ofstream logFile;
    int mainChainLength;

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;

public:
    PeerNode() : miningTimer(nullptr) {} // Constructor
    virtual ~PeerNode();
    int numSeeds;
    int numPeers;
    int maxPeersSel;
    bool isadversary;
    void registerWithSeed(int seedIndex);
    void PeerListasker();
    void connectToPeers();
    void setuplivness();
    void livnessTest();
    void mineBlock();
    bool validateAndAddBlock(Block block);
    string calculateHash(const Block& block);
    void broadcastBlock(const Block& block);
    void syncBlockchain();
    void floodNetwork();
    string sha256(const string& str);
    void gossip();
    void handleGossip(MyMessage *msg);
    void recordStatistics();
    void logBlockchain();

    vector<int>selectedSeeds;
    set<int> peerlist;
    set<int> connectedpeers;
    int receivedNumPlist=0;
    map<int,int>livness;
    map<int,simtime_t>livnessTimestamp;
    int gossip_gen=0;
    map<int,bool>ML;



};

Define_Module(PeerNode);

inline std::string to_hex(size_t hash) {
    std::stringstream ss;
    ss << std::hex << std::setw(16) << std::setfill('0') << hash;
    return ss.str();
}

inline std::string cal_hash(std::string str){
    std::hash<std::string> hasher;
    size_t hash_value = hasher(str);
    std::string hex_hash = to_hex(hash_value);
    return hex_hash;
}

PeerNode::~PeerNode()
{
    cancelAndDelete(miningTimer);
}

void PeerNode::initialize()
{
    string test="Khushal";
    EV<< "Hash of " << test << " is " << cal_hash(test) << endl;
    int adv_index=par("adv");
    EV<<"adv_index "<<adv_index<<endl;
    if(adv_index==getIndex()){
        isadversary=true;
    }
    else{
        isadversary=false;
    }

    numSeeds = par("ns");
    numPeers = par("np");
    maxPeersSel = 4;

    Block genesisBlock;
    genesisBlock.prevHash = "0000000000000000";
    genesisBlock.merkleRoot = "0000000000000000";
    genesisBlock.timestamp = time(nullptr);
    genesisBlock.height = 0;
    genesisBlock.hash = "0x9e1c";
    blockchain.push_back(genesisBlock);
    currentHash = genesisBlock.hash;
    mainChainLength = 1;

    nodeHashPower = par("hashPower").doubleValue();
    interarrivalTime = par("interarrivalTime").doubleValue();
    isAdversary = isadversary;  // Use the existing isadversary flag
    floodingRate = par("floodingRate").intValue();


    miningTimer = new cMessage("miningTimer");
    scheduleAt(simTime() + exponential(interarrivalTime / nodeHashPower), miningTimer);

    blocksMined = 0;
    totalBlocksMined = 0;

    miningPowerUtilization.setName("miningPowerUtilization");
    adversaryFraction.setName("adversaryFraction");
    lastRecordTime = simTime();

    string logFileName = "peer_" + to_string(getIndex()) + "_log.txt";
    logFile.open(logFileName);
    logFile << "Peer " << getIndex() << " initialized with genesis block: " << genesisBlock.hash << endl;



    // make [n/2] + 1 random connections to seeds
    vector<int> seedIndices(numSeeds);
    for(int i = 0; i < numSeeds; i++) {
        seedIndices[i] = i;
    }
    std::random_device rd; // Obtain a random number from hardware
    std::default_random_engine eng(rd()); // Seed the generator

    shuffle(seedIndices.begin(), seedIndices.end(), eng);
    EV<< "Seed indices: ";
    for (int i = 0; i < numSeeds; i++) {
        EV << seedIndices[i] << " ";
    }
    EV << endl;



    for (int i = 0; i < numSeeds/2 + 1; i++) {

        selectedSeeds.push_back(seedIndices[i]);

        cModule *seed = getParentModule()->getSubmodule("seed", seedIndices[i]);
        // add gate and connect to seed

        gate("out", seedIndices[i])->connectTo(seed->gate("in",getIndex()));

        seed->gate("out", getIndex())->connectTo(gate("in", seedIndices[i]));

        registerWithSeed(seedIndices[i]);

    }




        MyMessage *askpeerlist = new MyMessage();
        askpeerlist->messageType = "askPeerList";
        askpeerlist->content = "peer";

        // delay for 1 second and ask for the peer list
        scheduleAt(simTime() + 1, askpeerlist);







}

string PeerNode::calculateHash(const Block& block)
{
    string blockData = block.prevHash + block.merkleRoot + to_string(block.timestamp);
    return sha256(blockData);
}

string PeerNode::sha256(const string& str)
{
    return cal_hash(str).substr(0, 16); // Return only the first 16 characters (64 bits)
}


void PeerNode::handleMessage(cMessage *msg)
{
    if (msg == miningTimer) {
        mineBlock();
        cancelEvent(miningTimer);
        scheduleAt(simTime() + exponential(interarrivalTime / nodeHashPower), miningTimer);
    } else if (msg->isSelfMessage() && strcmp(msg->getName(), "floodTimer") == 0) {
        floodNetwork();
        scheduleAt(simTime() + exponential(1.0 / floodingRate), msg);
    } else {
        MyMessage *m = check_and_cast<MyMessage *>(msg);
        if (m->messageType == "block") {
            Block receivedBlock;
            receivedBlock.prevHash = m->prevHash;
            receivedBlock.merkleRoot = m->merkleRoot;
            receivedBlock.timestamp = m->blockTimestamp;
            receivedBlock.hash = m->blockHash;
            receivedBlock.height = blockchain.back().height + 1;

            if (validateAndAddBlock(receivedBlock)) {
                logFile << "Received valid block: " << receivedBlock.hash << " at height " << receivedBlock.height << endl;
                broadcastBlock(receivedBlock);

                // Cancel the current mining timer and reschedule
                cancelEvent(miningTimer);
                scheduleAt(simTime() + exponential(interarrivalTime / nodeHashPower), miningTimer);
            } else {
                logFile << "Received invalid block: " << receivedBlock.hash << endl;
            }
        } else if (m->messageType == "peerList") {
            peerlist.insert(m->peerlist.begin(), m->peerlist.end());
            receivedNumPlist++;

            if (receivedNumPlist == selectedSeeds.size()) {
                EV << "final Peer list at peer " << getIndex() << " : ";
                for (int peer : peerlist) {
                    EV << peer << " ";
                }
                EV << endl;
                connectToPeers();
            }
        } else if (m->messageType == "askPeerList") {
            PeerListasker();
            setuplivness();
        } else if (m->messageType == "do livness test") {
            livnessTest();
            EV << "livness test " << getIndex() << endl;
            MyMessage *livnessMessage = new MyMessage();
            livnessMessage->messageType = "do livness test";
            livnessMessage->address = getIndex();
            scheduleAt(simTime() + 13, livnessMessage);
        } else if (m->messageType == "livness Request") {
            MyMessage *livnessMessage = new MyMessage();
            livnessMessage->messageType = "livness Reply";
            livnessMessage->address = getIndex();
            livnessMessage->timestamp = m->timestamp;
            livnessMessage->sender_address = m->address;
            send(livnessMessage, "peerout", m->address);
        } else if (m->messageType == "livness Reply") {
            EV << "livness Reply from " << m->address << endl;
            if (livnessTimestamp[m->address] == m->timestamp && m->sender_address == getIndex()) {
                livness[m->address] = 0;
            }
        } else if (m->messageType == "do gossip") {
            EV << "do gossip " << getIndex() << endl;
            gossip();
        } else if (m->messageType == "gossip") {
            EV << "gossip received from " << m->address << " : " << m->content << endl;
            handleGossip(m);
        }

        delete m;
    }

    // Record statistics periodically
    if (simTime().dbl() - lastRecordTime.dbl() > 60.0) {
        recordStatistics();
        lastRecordTime = simTime();
    }
}


void PeerNode::mineBlock()
{
    Block newBlock;
    newBlock.prevHash = currentHash;
    newBlock.merkleRoot = sha256("merkle_root_placeholder");
    newBlock.timestamp = time(nullptr);
    newBlock.height = blockchain.back().height + 1;

    string blockData = newBlock.prevHash + newBlock.merkleRoot + to_string(newBlock.timestamp);
    newBlock.hash = sha256(blockData);

    // Simplified PoW condition (adjust difficulty as needed)
//     if (newBlock.hash.substr(0, 1) == "0") {  // Uncomment this line for a very easy difficulty
    if (true) {  // Always accept the block for testing purposes
        if (validateAndAddBlock(newBlock)) {
            broadcastBlock(newBlock);
            blocksMined++;
            logFile << "Mined new block: " << newBlock.hash << " at height " << newBlock.height << endl;
        }
    }

    totalBlocksMined++;

    // Log mining attempt
    logFile << "Mining attempt: " << newBlock.hash << " at height " << newBlock.height << endl;

    // Schedule next mining attempt
    scheduleAt(simTime() + exponential(interarrivalTime / nodeHashPower), miningTimer);
}

bool PeerNode::validateAndAddBlock(Block block)
{
    if (block.prevHash == currentHash &&
        abs(static_cast<long>(block.timestamp) - static_cast<long>(time(nullptr))) <= 3600) {

        blockchain.push_back(block);
        currentHash = block.hash;
        mainChainLength = max(mainChainLength, block.height + 1);

        logFile << "Added block to chain: " << block.hash << " at height " << block.height << endl;

        // Process pending queue
        while (!pendingQueue.empty()) {
            Block pendingBlock = pendingQueue.front();
            pendingQueue.pop();
            if (pendingBlock.prevHash == currentHash) {
                validateAndAddBlock(pendingBlock);
            } else {
                pendingQueue.push(pendingBlock);
                break;
            }
        }
        return true;
    } else {
        pendingQueue.push(block);
        return false;
    }
}



void PeerNode::broadcastBlock(const Block& block)
{
    for (int peerIndex : connectedpeers) {
        MyMessage *blockMsg = new MyMessage();
        blockMsg->messageType = "block";
        blockMsg->prevHash = block.prevHash;
        blockMsg->merkleRoot = block.merkleRoot;
        blockMsg->blockTimestamp = block.timestamp;
        blockMsg->blockHash = block.hash;
        send(blockMsg, "peerout", peerIndex);
    }
    logFile << "Broadcasted block: " << block.hash << " to " << connectedpeers.size() << " peers" << endl;
}

void PeerNode::syncBlockchain()
{
    // Request blockchain from a random connected peer
    if (!connectedpeers.empty()) {
        int randomPeer = *next(connectedpeers.begin(), rand() % connectedpeers.size());
        MyMessage *syncMsg = new MyMessage();
        syncMsg->messageType = "syncRequest";
        syncMsg->address = getIndex();
        send(syncMsg, "peerout", randomPeer);
    }
}

void PeerNode::floodNetwork()
{
    if (!isAdversary) return;

    Block invalidBlock;
    invalidBlock.prevHash = "invalid_prev_hash";
    invalidBlock.merkleRoot = "invalid_merkle_root";
    invalidBlock.timestamp = time(nullptr);
    invalidBlock.hash = "invalid_hash";

    for (int target : connectedpeers) {
        MyMessage *blockMsg = new MyMessage();
        blockMsg->messageType = "block";
        blockMsg->prevHash = invalidBlock.prevHash;
        blockMsg->merkleRoot = invalidBlock.merkleRoot;
        blockMsg->blockTimestamp = invalidBlock.timestamp;
        blockMsg->blockHash = invalidBlock.hash;
        send(blockMsg, "peerout", target);
    }
}


void PeerNode::recordStatistics()
{
    double utilization = (totalBlocksMined > 0) ? static_cast<double>(blockchain.size()) / totalBlocksMined : 0.0;
    miningPowerUtilization.record(utilization);

    double adversaryFrac = (blockchain.size() > 0) ? static_cast<double>(blocksMined) / blockchain.size() : 0.0;
    if (isAdversary) {
        adversaryFraction.record(adversaryFrac);
    }

    logFile << "PLOT_DATA,"
            << simTime() << ","
            << utilization << ","
            << adversaryFrac << ","
            << blockchain.size() << ","
            << totalBlocksMined << endl;
}

void PeerNode::handleGossip(MyMessage *msg){
    // hash and keep in map hash,true
    // if hash already exists, ignore
    // if hash does not exist,
    //    hash,true
    int hashval = hash<string>{}(SIMTIME_STR(msg->timestamp)+to_string(msg->address)+msg->content);
    if(ML.find(hashval)==ML.end()){
        ML[hashval]=true;
        // forward the gossip to all adjacent peers
        for(int peerIndex : connectedpeers){
            if(peerIndex==getIndex()){
                continue;
            }
            MyMessage *gossipMessage = new MyMessage();
            gossipMessage->messageType = "gossip";
            gossipMessage->timestamp = simTime();
            gossipMessage->address = getIndex();
            // generate random gossip content

            gossipMessage->content = "gossip content"+to_string(rand()%100);
            send(gossipMessage, "peerout", peerIndex);

        }
    }


    // forward the gossip to all adjacent peers

}

void PeerNode::logBlockchain()
{
    logFile << "Current blockchain state:" << endl;
    for (const auto& block : blockchain) {
        logFile << "Height: " << block.height << ", Hash: " << block.hash << ", PrevHash: " << block.prevHash << endl;
    }
}

void PeerNode::gossip(){
    gossip_gen++;
    if(gossip_gen<=10){
        // send gossip to all adjacent peers
        for(int peerIndex : connectedpeers){
              if(peerIndex==getIndex()){
                continue;
            }
            MyMessage *gossipMessage = new MyMessage();
            gossipMessage->messageType = "gossip";
            gossipMessage->timestamp = simTime();
            gossipMessage->address = getIndex();
            // generate random gossip content

            gossipMessage->content = "gossip content"+to_string(rand()%100);
            send(gossipMessage, "peerout", peerIndex);

        }
    }
    // schedule next gossip
    MyMessage *gossipMessage = new MyMessage();
    gossipMessage->messageType = "do gossip";
    gossipMessage->address = getIndex();
    scheduleAt(simTime() + 5, gossipMessage);
}
void PeerNode::livnessTest(){


    for(int peerIndex : connectedpeers){
        if(peerIndex==getIndex()){
            continue;
        }
        livness[peerIndex]++;
        if(livness[peerIndex]>3){

            // notify the seeds of the dead peer
            for(int seedIndex : selectedSeeds) {
                MyMessage *peerListMessage = new MyMessage();
                peerListMessage->messageType = "Dead Node";
                peerListMessage->content = "";
                peerListMessage->address = getIndex();
                peerListMessage->sender_address = peerIndex;
                send(peerListMessage, "peerout", seedIndex);

            }
            peerlist.erase(peerIndex);

            continue;
        }
        livnessTimestamp[peerIndex]=simTime();
        MyMessage *livnessMessage = new MyMessage();
        livnessMessage->messageType = "livness Request";
        livnessMessage->address = getIndex();


        livnessMessage->timestamp =livnessTimestamp[peerIndex];


        send(livnessMessage, "peerout", peerIndex);
    }
}
void PeerNode::setuplivness(){
    for(int peerIndex : peerlist){

        livness[peerIndex]=0;
    }
    MyMessage *livnessMessage = new MyMessage();
    livnessMessage->messageType = "do livness test";
    livnessMessage->address = getIndex();
    scheduleAt(simTime() + 1, livnessMessage);

}
void PeerNode::connectToPeers(){
    // Ramdomly select maxPeersSel peers to connect to
    maxPeersSel = min(4,(int)peerlist.size());
    vector<int> peerIndices((int)peerlist.size());

    for(int i = 0; i < peerlist.size(); i++) {
        peerIndices[i] = i;
    }
    std::random_device rd; // Obtain a random number from hardware
    std::default_random_engine eng(rd()); // Seed the generator

    shuffle(peerIndices.begin(), peerIndices.end(), eng);

    for (int i = 0; i < maxPeersSel; i++) {
        int peerIndex = peerIndices[i];
        connectedpeers.insert(peerIndex);
        cModule *peer = getParentModule()->getSubmodule("peer", peerIndex);
        // add gate and connect to peer
        // gate("peerout", peerIndex)->connectTo(peer->gate("peerin",getIndex()));

        // peer->gate("peerout", getIndex())->connectTo(gate("peerin", peerIndex));
        // if not already connected
        if(gate("peerout", peerIndex)->getPathEndGate()->getPreviousGate() == nullptr) {
            gate("peerout", peerIndex)->connectTo(peer->gate("peerin", getIndex()));
        }
        if(peer->gate("peerout", getIndex())->getPathEndGate()->getPreviousGate() == nullptr) {
            peer->gate("peerout", getIndex())->connectTo(gate("peerin", peerIndex));
        }
    }


}
void PeerNode:: PeerListasker() {
    for(int seedIndex : selectedSeeds) {
        MyMessage *peerListMessage = new MyMessage();
        peerListMessage->messageType = "peerList";
        peerListMessage->content = "";
        peerListMessage->address = getIndex();
        send(peerListMessage, "out", seedIndex);
    }


}


void PeerNode::registerWithSeed(int seedIndex) {
    // send a message to the seed to register
    MyMessage *msg = new MyMessage();
    msg->messageType = "register";
    msg->content = "peer";
    msg->address = getIndex();

    send(msg, "out", seedIndex);
}

void PeerNode::finish()
{
    logBlockchain();
    logFile << "SUMMARY_START" << endl;
    logFile << "PeerID: " << getIndex() << endl;
    logFile << "TotalBlocksMined: " << blocksMined << endl;
    logFile << "BlockchainLength: " << blockchain.size() << endl;
    logFile << "IsAdversary: " << (isAdversary ? "true" : "false") << endl;
    logFile << "FinalMiningPowerUtilization: " << (totalBlocksMined > 0 ? static_cast<double>(blockchain.size()) / totalBlocksMined : 0.0) << endl;
    logFile << "SUMMARY_END" << endl;

    logFile.close();
}