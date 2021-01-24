#ifndef CTREE_HPP
#define CTREE_HPP
#include<iostream>
#include<fstream>
#include<vector>
#include<algorithm>
#include<sstream>
#include<unordered_map>

class CTree;
using namespace std;
class CNode{
    public:
        friend class CTree;
        friend ostream& operator<<( ostream&, const CNode& );
    private:
        int qubitId;
        bool isTerminal = true;
        unsigned int traverse = 0;
        vector< int> edges;
};
class CTree{
    public:
        void clear();
        void setCNodesNum( int);
        void setCNodeQubitId( int, int);
        void setCNodeEdge( int, int);
        void setIsTerminal( int);
        bool getIsTerminal( int);
        bool getIsLeaf( int);
        int  getQid( int);
        void trim();
        void setIsSteinerNode( int);
        void setEdge(int, int);
        void cNodePushBack( CNode);
        void traverseIncrease();
        void buildCTrees(int, vector<CTree>&, bool);
        void buildR(vector< pair< int, int> >&, bool);
        int  getRootQId();

        friend ostream& operator<<( ostream&, const CTree& );
    private:
        unsigned int traverse = 0;
        vector< CNode> cNodes;
};

ostream& operator<<( ostream&, const CTree& );
ostream& operator<<( ostream&, const CNode& );


#endif
