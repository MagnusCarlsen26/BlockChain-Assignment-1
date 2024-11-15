//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//
simple SeedNode
{
    parameters:
        int ns = default(1);  // Total number of seed nodes
        int np = default(1);  // Total number of peer nodes
    gates:
        input in[np];
        output out[np];
}

simple PeerNode
{
    parameters:
        int ns = default(1);  // Total number of seed nodes
        int np = default(1);  // Total number of peer nodes
        int adv=default(0);
        double hashPower = default(uniform(0.1, 1.0));
        double interarrivalTime = default(600);  // 10 minutes on average
        bool isAdversary = default(false);
        int floodingRate = default(1);
    gates:
        input in[ns];
        output out[ns];
        input peerin[np];
        output peerout[np];
    
        
}

network Net
{
    parameters:
        int numSeeds;
        int numPeers;
        int adv=intuniform(0,numPeers-1);
    submodules:
        seed[numSeeds]: SeedNode {
            parameters:
                ns = parent.numSeeds;
                np = parent.numPeers;
              
        }
        peer[numPeers]: PeerNode {
            parameters:
                ns = parent.numSeeds;
                np = parent.numPeers;
                adv= parent.adv;
        }
    connections allowunconnected:
        // Connect peers to seeds
//        for i=0..numPeers-1, for j=0..numSeeds-1 {
//            peer[i].out++ --> {delay=100ms;} --> seed[j].in++;
//            peer[i].in++ <-- {delay=100ms;} <-- seed[j].out++;
//        }
        // Connect peers to peers (will be established dynamically in the simulation)
}