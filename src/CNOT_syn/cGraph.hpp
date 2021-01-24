#ifndef CGARPH_HPP
#define CGARPH_HPP
#include<iostream>
#include<fstream>
#include<vector>
#include<algorithm>
#include<sstream>
#include<unordered_map>
#include<unordered_set>
#include<boost/range/adaptor/reversed.hpp>
#include<cmath>
#include"circuit.hpp"
#include"cTree.hpp"

class CGraph;
using namespace std;
struct pair_hash {
    template <class T1, class T2>
    std::size_t operator () (const std::pair<T1,T2> &p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);

        // Mainly for demonstration purposes, i.e. works but is overly simple
        // In the real world, use sth. like boost.hash_combine
        return h1 ^ h2;  
    }
};
class Qubit{
    public:
        void initQubit(int, int, int);
        void neighborsPush_back(int);
        vector<int>& getneighbors();
        friend class CGraph;
        friend ostream& operator<<(ostream&, Qubit&);
    private:
        vector< int > neighbors;
        int id;
        int x;
        int y;

};
class CGraph{
    public:
        Qubit& getQubit( int);
        void setQubitsNum( int);
        vector< pair< int, int> >& rowOperations(int, vector< int>&, bool );
        void buildCTree( int, vector< int>&, vector< int>& );
        void buildCTrees( int);// bfs
        int  getDistance( int, int);
        unordered_map< pair<int, int>, int, pair_hash>& getCoordinate2qubit();

        friend istream& operator>>(istream&, CGraph&);
        friend ostream& operator<<(ostream&, CGraph&);
    private:
        vector<Qubit> qubits;
        unordered_map< pair<int, int>, int, pair_hash> coordinate2qubit;
        unordered_map< int, int> qubit2CNotNode;
        CTree CNotRSMT;
        vector<CTree> cTrees;
};
istream& operator>>(istream&, CGraph&);
ostream& operator<<(ostream&, Qubit&);
ostream& operator<<(ostream&, CGraph&);


#endif
