#include"cGraph.hpp"

void Qubit::initQubit(int _id, int _x, int _y){id=_id; x=_x; y=_y; };

void Qubit::neighborsPush_back(int i){
    neighbors.push_back(i);
}
vector<int>& Qubit::getneighbors(){
    return neighbors;
}
Qubit& CGraph::getQubit( int i){
    return qubits[i];
}
void CGraph::setQubitsNum( int i){
    qubits.resize(i);
}
void CGraph::buildCTree(int ctrl, vector< int>& targets, vector< int>& CNodes){
    CNotRSMT.clear();
    CNotRSMT.setCNodesNum(1+targets.size()+CNodes.size());
    CNotRSMT.setCNodeQubitId(0, ctrl);
    qubit2CNotNode.insert({ctrl,0});
    //vCNotRSMT.setIsTerminal(0));
    int count = 0;
    for( auto& target: targets){
        CNotRSMT.setCNodeQubitId(++count, target);
        qubit2CNotNode.insert({target, count});
        // CNotRSMT.setIsTerminal(count);
    }
    for( auto& CNode: CNodes){
        CNotRSMT.setCNodeQubitId(++count, CNode);
        qubit2CNotNode.insert({CNode, count});
        CNotRSMT.setIsSteinerNode(count);
    }

}
vector< pair< int, int> >& CGraph::rowOperations(int ctrl, vector< int>& targets, bool u){
    static vector< pair< int, int> > ctrlTargetPairs;
    static unordered_set< pair<int, int>, pair_hash > existCoordinate;
    static unordered_set< pair<int, int>, pair_hash > existEdges;
    static vector< int> CNodes;// need to check if steiner node is in the set
    static vector< pair<int, int> > Edges;// need to check if Edges is in the set

    ctrlTargetPairs.clear();
    existCoordinate.clear();
    existEdges.clear();
    CNodes.clear();
    Edges.clear();
    qubit2CNotNode.clear();
    CNotRSMT.clear();
    cTrees.clear();

    if ( targets.size() == 0)return ctrlTargetPairs;

    // init ctrl
    int count = 0;
    int x[MAXD];
    int y[MAXD];
    x[0] = qubits[ctrl].x;
    y[0] = qubits[ctrl].y;
    existCoordinate.insert({x[0], y[0]});

    // init targets
    count++;
    for( auto& target: targets){
        x[count] = qubits[target].x;
        y[count] = qubits[target].y;
        existCoordinate.insert({x[count], y[count]});
        count++;
    }

    if ( count <= 1)return ctrlTargetPairs;
    Tree fluteTree = flute(count,x,y,ACCURACY);
    // init CNodes
    for (int i=fluteTree.deg; i<2*fluteTree.deg-2; i++){
        pair<int, int> CNodeCoordinate = {fluteTree.branch[i].x, fluteTree.branch[i].y};

        if( existCoordinate.find(CNodeCoordinate) != existCoordinate.end() ) continue;
        int CNode = coordinate2qubit[CNodeCoordinate];
        CNodes.push_back(CNode);
        existCoordinate.insert(CNodeCoordinate);
    }

    buildCTree(ctrl, targets, CNodes );
    pair< int, int> fluteEdge[2];
    int qubitId[2];
    int cNodeId[2];
    for (int i = 0; i < fluteTree.deg * 2 - 2; i++) {
        Branch& branch1 = fluteTree.branch[i];
        Branch& branch2 = fluteTree.branch[branch1.n];
        fluteEdge[0] = {branch1.x, branch1.y};
        fluteEdge[1] = {branch2.x, branch2.y};
        if ( fluteEdge[0] == fluteEdge[1]) continue;
        qubitId[0] = coordinate2qubit[fluteEdge[0]];
        qubitId[1] = coordinate2qubit[fluteEdge[1]];
        cNodeId[0] = qubit2CNotNode[qubitId[0]];
        cNodeId[1] = qubit2CNotNode[qubitId[1]];
        if( existEdges.find({cNodeId[0], cNodeId[1]}) != existEdges.end() ) continue;
        if( existEdges.find({cNodeId[1], cNodeId[0]}) != existEdges.end() ) continue;
        CNotRSMT.setCNodeEdge(cNodeId[0], cNodeId[1] );
        CNotRSMT.setCNodeEdge(cNodeId[1], cNodeId[0] );
    }
    CNotRSMT.traverseIncrease();
    CNotRSMT.buildCTrees(qubit2CNotNode[ctrl], cTrees, u);
    // cout << "CNotRSMT = \n" << CNotRSMT << endl;
    if ( u == false){
        for( auto& tree: boost::adaptors::reverse(cTrees)){
            // cout << "tree = \n" << tree << endl;
            vector< pair< int, int>> R;
            vector< pair< int, int>> R_prime;
            vector< pair< int, int>> R_star;
            tree.buildR(R, false);
            if ( R.size() == 0) continue;
            for( auto& [ctrl, target]: boost::adaptors::reverse(R)){
                if( ctrl != tree.getRootQId() )R_prime.push_back({ctrl, target});
            }
            R_star = R;
            R_star.insert(R_star.end(), R_prime.begin(), R_prime.end());
            ctrlTargetPairs.insert(ctrlTargetPairs.end(), R_star.begin(), R_star.end());
            for( auto& CNotGate: R_star){
                int targetQId = CNotGate.second;
                int targetCNodeId = qubit2CNotNode[targetQId];
                if( CNotRSMT.getIsTerminal(targetCNodeId) == false){
                    ctrlTargetPairs.push_back(CNotGate);
                }
            }
        }
    }
    else{
        assert(cTrees.size() == 1);
        auto& tree = cTrees[0];
        // cout << "before trim tree = \n" << tree << endl;
        vector< pair< int, int>> R;
        vector< pair< int, int>> R_prime;
        vector< pair< int, int>> R_star;
        tree.buildR(R, false);
        assert(R.size() != 0);
        for( auto& [ctrl, target]: boost::adaptors::reverse(R)){
            if( ctrl != tree.getRootQId() )R_prime.push_back({ctrl, target});
        }
        R_star = R;
        R_star.insert(R_star.end(), R_prime.begin(), R_prime.end());
        for( auto& step:R_star){
            //  << step.first << ' ' << step.second << endl;
        }
        ctrlTargetPairs.insert(ctrlTargetPairs.end(), R_star.begin(), R_star.end());

        R.clear();
        R_prime.clear();
        R_star.clear();
        tree.trim();

        // cout << "after trim tree = \n" << tree << endl;
        tree.buildR(R, true);
        for( auto& [ctrl, target]: boost::adaptors::reverse(R)){
            if( ctrl != tree.getRootQId() )R_prime.push_back({ctrl, target});
        }
        R_star = R;
        R_star.insert(R_star.end(), R_prime.begin(), R_prime.end());
        for( auto& step:R_star){
            // cout << step.first << ' ' << step.second << endl;
        }
        ctrlTargetPairs.insert(ctrlTargetPairs.end(), R_star.begin(), R_star.end());
    }

    free(fluteTree.branch);
    auto ctrlTargetPairs_clone = ctrlTargetPairs;
    ctrlTargetPairs.clear();
    for( auto& [ctrl, target]: ctrlTargetPairs_clone){
        vector<int> pass;
        pair<int, int> ctrlCoordinate= {qubits[ctrl].x, qubits[ctrl].y};
        pair<int, int> targetCoordinate= {qubits[target].x, qubits[target].y};
        while( ctrlCoordinate.first != targetCoordinate.first){
            (ctrlCoordinate.first < targetCoordinate.first)?ctrlCoordinate.first++:ctrlCoordinate.first--;
            pass.push_back(coordinate2qubit[ctrlCoordinate]);
        }
        while( ctrlCoordinate.second != targetCoordinate.second){
            (ctrlCoordinate.second < targetCoordinate.second)?ctrlCoordinate.second++:ctrlCoordinate.second--;
            pass.push_back(coordinate2qubit[ctrlCoordinate]);
        }
        for(int i = (int)pass.size()-1; i >0; --i){
            ctrlTargetPairs.push_back({pass[i-1], pass[i]});
        }
        ctrlTargetPairs.push_back({ctrl, pass[0]});
        for(int i = 0; i < (int)pass.size()-1; ++i){
            ctrlTargetPairs.push_back({pass[i], pass[i+1]});
        }
        for(int i = (int)pass.size()-2; i >0; --i){
            ctrlTargetPairs.push_back({pass[i-1], pass[i]});
        }
        if( pass.size() != 1)   ctrlTargetPairs.push_back({ctrl, pass[0]});
        for(int i = 0; i < (int)pass.size()-2; ++i){
            ctrlTargetPairs.push_back({pass[i], pass[i+1]});
        }
    }
    return ctrlTargetPairs;
}
int  CGraph::getDistance( int q1, int q2){
    int xDistance = abs((qubits[q1].x-qubits[q2].x));
    int yDistance = abs((qubits[q1].y-qubits[q2].y));
    return xDistance + yDistance;

}

unordered_map< pair<int, int>, int, pair_hash>& CGraph::getCoordinate2qubit(){
    return coordinate2qubit;
}
ostream& operator<<(ostream& os, Qubit& qubit){
    os << qubit.id << " (" << qubit.x << ',' << qubit.y << ") {";
    for( auto& n: qubit.neighbors){
        os << n;
        if (n != *qubit.neighbors.rbegin()) os << ',';
        else os << '}';
    }
    return os;
}

istream& operator>>(istream& is, CGraph& cgraph){
    string header;
    getline(is, header);
    string delimiter = "=";
    int n = std::stoi(header.substr(header.find(delimiter)+1, header.length()));
    cgraph.setQubitsNum(n);
    string qubit;
    while(getline(is, qubit)){
        int id;
        string cooridate;
        string neighbors;
        auto leftParentheses = qubit.find("(");
        auto rightParentheses = qubit.find(")");
        auto leftBrackets = qubit.find("{");
        auto rightBrackets = qubit.find("}");
        id = std::stoi(qubit.substr(0, leftParentheses));
        cooridate = qubit.substr(leftParentheses+1, rightParentheses-1);
        neighbors = qubit.substr(leftBrackets+1, rightBrackets-1);
        replace(cooridate.begin(), cooridate.end(), ',', ' ');
        replace(neighbors.begin(), neighbors.end(), ',', ' ');
        stringstream iss1( cooridate );
        int x;
        int y;
        iss1 >> x >> y;
        cgraph.qubits[id].initQubit(id, x, y);
        cgraph.coordinate2qubit.insert({{x,y}, id});
        stringstream iss2( neighbors );
        int qb;
        while ( iss2 >> qb ){
            cgraph.qubits[id].neighborsPush_back(qb);
        }
    }
    return is;
}

ostream& operator<<(ostream& os, CGraph& cgraph){
    for( auto& qb: cgraph.qubits){
        os << qb << '\n';
    }
    os << '\n';
    for ( const auto& p: cgraph.coordinate2qubit){
        os << '(' << p.first.first << ',' << p.first.second << ") " << p.second <<'\n';
    }
    return os;
}