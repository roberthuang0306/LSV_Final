#ifndef MATRIX_HPP
#define MATRIX_HPP

#include<iostream>
#include<fstream>
#include<random>
#include<vector>
#include<cmath>
#include<bitset>
#include"circuit.hpp"
class Circuit;

using namespace std;
class Matrix{
    public:
        Matrix( int, int);
        Matrix( pair< int, int>);
        Matrix();
        Circuit circuit( int);
        Circuit circuit();
        Circuit Lwr_CNOT_Synth( int);
        pair<int, int> size();
        size_t columnSize();
        size_t rowSize();
        void columnResize( int);
        void rowOperation( int, int);
        void rowResize( int);
        void transpose();
        bool operator==(const Matrix&);
        vector< bool>& operator[]( int);
        friend ostream& operator<< ( ostream&, const Matrix&);

    private:
        vector< vector< bool>> _matrix;
};
ostream& operator<< ( ostream&, const Matrix&);
#endif