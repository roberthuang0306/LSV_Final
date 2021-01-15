#ifndef CIRCUIT_HPP
#define CIRCUIT_HPP

#include<iostream>
#include<fstream>
#include<random>
#include<vector>
#include<algorithm>
#include<tuple>
#include"matrix.hpp"
class Matrix;

using namespace std;
class Circuit{
    public:
        Circuit();
        Circuit( int);
        void switchCtrlTarget();
        void reverse();
        void gateNameReverse();
        void push_back(pair< int, int>);
        void setInputNum( int);
        int  getInputNum();
        int  getCircuitSize();
        Matrix matrix();
        Circuit& operator+=(const Circuit&);
        bool operator==(const Circuit&);
        friend ostream& operator<< ( ostream&, const Circuit&);
        friend istream& operator>> ( istream&, Circuit&);
    private:
        vector< pair< int, int>> _circuit;
        int inputNum;
};
ostream& operator<< ( ostream&, const Circuit&);
istream& operator>> ( istream&, Circuit&);

template< typename T1, typename T2>
ostream& operator<< ( ostream&, const pair< T1, T2>& _pair);
#endif