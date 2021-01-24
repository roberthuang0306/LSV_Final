#include"cTree.hpp"
#include<queue>
#include<stack>

void CTree::clear(){
    traverse = 0;
    cNodes.clear();
}
void CTree::setCNodesNum(int n){
    cNodes.resize(n);
}
void CTree::setCNodeQubitId(int id, int qId){
    cNodes[id].qubitId = qId;
}
void CTree::setCNodeEdge(int id, int edgeId){
    cNodes[id].edges.push_back(edgeId);
}
void CTree::setIsTerminal(int id){
    cNodes[id].isTerminal = true;
}
bool CTree::getIsTerminal(int id){
    return cNodes[id].isTerminal;
}
bool CTree::getIsLeaf(int id){
    return (cNodes[id].edges.size() == 0 && cNodes[id].isTerminal == true);
}
void CTree::trim(){
    stack<int> DFSstack;
    stack<int> rDFS;
    int rootDirectedId = 0;
    DFSstack.push(rootDirectedId);
    while(DFSstack.size() != 0){
        int visitingDerictedCNodeId = DFSstack.top();
        rDFS.push(visitingDerictedCNodeId);
        DFSstack.pop();
        for( auto& derictedEdge: cNodes[visitingDerictedCNodeId].edges){
            DFSstack.push(derictedEdge);
        }
    }
    while(rDFS.size() != 0){
        int visitingDerictedCNodeId = rDFS.top();
        rDFS.pop();
        for( auto& derictedEdge: cNodes[visitingDerictedCNodeId].edges){
            if ( getIsLeaf(derictedEdge)) derictedEdge = -1;
        }
        auto edgesClone = cNodes[visitingDerictedCNodeId].edges;
        cNodes[visitingDerictedCNodeId].edges.clear();
        for( auto& edge: edgesClone){
            if( edge >= 0 ) cNodes[visitingDerictedCNodeId].edges.push_back(edge);
        }
    }
}
void CTree::setIsSteinerNode(int id){
    cNodes[id].isTerminal = false;
}
void CTree::cNodePushBack( CNode cNode){
    cNodes.push_back(cNode);
}
void CTree::traverseIncrease(){
    traverse++;
}
void CTree::buildCTrees(int rootId, vector<CTree>& cTrees, bool u){
    queue<int> BFSQueue;
    unordered_map< int, int> qubit2directedCNotNode;
    int cTreeId = cTrees.size();
    cTrees.push_back(CTree());
    int count = 0;

    cTrees[cTreeId].cNodePushBack(CNode());
    qubit2directedCNotNode.insert({cNodes[rootId].qubitId, count});
    cTrees[cTreeId].cNodes[count].qubitId = cNodes[rootId].qubitId;
    cTrees[cTreeId].cNodes[count].isTerminal = cNodes[rootId].isTerminal;
    ++count;

    BFSQueue.push(rootId);
    cNodes[rootId].traverse = traverse;

    while(BFSQueue.size() != 0){
        int visitingCNodeId = BFSQueue.front();
        int visitingdirectedCNodeId = qubit2directedCNotNode[ cNodes[visitingCNodeId].qubitId ];
        BFSQueue.pop();

        if(cNodes[visitingCNodeId].isTerminal == true && visitingCNodeId != rootId && u == false){
            buildCTrees(visitingCNodeId, cTrees, u);
        }
        else{
            vector< int>& edgeCNodes = cNodes[visitingCNodeId].edges;
            for( auto& edgeId: edgeCNodes){
                if ( cNodes[edgeId].traverse == traverse) continue;
                else{
                    cTrees[cTreeId].cNodePushBack(CNode());
                    qubit2directedCNotNode.insert({cNodes[edgeId].qubitId, count});
                    cTrees[cTreeId].cNodes[count].qubitId = cNodes[edgeId].qubitId;
                    cTrees[cTreeId].cNodes[count].isTerminal = cNodes[edgeId].isTerminal;
                    cTrees[cTreeId].cNodes[visitingdirectedCNodeId].edges.push_back(count);
                    ++count;

                    BFSQueue.push(edgeId);
                    cNodes[edgeId].traverse = traverse;
                }
            }
        }
    }
}
void CTree::buildR(vector< pair< int, int> >& R, bool star=false){
    stack<int> DFSstack;
    int RSizeOld = R.size();
    int rootDirectedId = 0;
    DFSstack.push(rootDirectedId);
    while(DFSstack.size() != 0){
        int visitingDerictedCNodeId = DFSstack.top();
        DFSstack.pop();
        for( auto& derictedEdge: cNodes[visitingDerictedCNodeId].edges){
            R.push_back({cNodes[visitingDerictedCNodeId].qubitId, cNodes[derictedEdge].qubitId});
            DFSstack.push(derictedEdge);
        }
        if( star == true){
            if( cNodes[visitingDerictedCNodeId].isTerminal == true && visitingDerictedCNodeId != rootDirectedId){
                auto& derictedEdge = *cNodes[visitingDerictedCNodeId].edges.rbegin();
                R.push_back({cNodes[derictedEdge].qubitId, cNodes[visitingDerictedCNodeId].qubitId});
                R.push_back({cNodes[visitingDerictedCNodeId].qubitId, cNodes[derictedEdge].qubitId});
            }
        }
    }
    reverse(R.begin()+RSizeOld, R.end());
}
int  CTree::getRootQId(){
    return cNodes[0].qubitId;
}
int CTree:: getQid( int id){
    return cNodes[id].qubitId;
}
ostream& operator<<( ostream& os, const CTree& cTree){
    int count = 0;
    for( const auto& cNode: cTree.cNodes){
        os << "cNodeId = " << count++;
        os << cNode << '\n';
    }
    return os;
}
ostream& operator<<( ostream& os, const CNode& cNode){
    os << " qId = " << cNode.qubitId;
    os << " isTerminal = " << cNode.isTerminal;
    os << " tarverse = " << cNode.traverse;
    os << " edges = {";
    for ( const auto& edge: cNode.edges){
        os << edge;
        if ( edge != *cNode.edges.rbegin()) os << ',';
    }
    os << "}\n";
    return os;
}