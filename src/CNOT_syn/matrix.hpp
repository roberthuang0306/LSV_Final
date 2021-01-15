#ifndef MATRIX_HPP
#define MATRIX_HPP

#include<iostream>
#include<fstream>
#include<random>
#include<vector>
#include<cmath>
#include<bitset>
#include<sstream>
#include"cGraph.hpp"
#include"circuit.hpp"
extern "C" {
#include "flute/flute.h"
}
class Circuit;
class CGraph;

using namespace std;
class Matrix{
    public:
        Matrix( int, int);
        Matrix( pair< int, int>);
        Matrix();
        Circuit circuit( int);
        Circuit circuit();
        Circuit circuit( CGraph*);
        Circuit Lwr_CNOT_Synth( int, CGraph*, bool);
        pair<int, int> size();
        size_t columnSize();
        size_t rowSize();
        void columnResize( int);
        void rowResize( int);
        void rowOperation( int, int);
        void transpose();
        void totalReverse();
        bool operator==(const Matrix&);
        vector< bool>& operator[]( int);
        friend ostream& operator<< ( ostream&, const Matrix&);
        friend istream& operator>> ( istream&, Matrix&);

    private:
        vector< vector< bool>> _matrix;
};
ostream& operator<< ( ostream&, const Matrix&);
istream& operator>> ( istream&, Matrix&);
#endif