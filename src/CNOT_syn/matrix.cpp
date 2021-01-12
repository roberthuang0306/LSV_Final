#include"matrix.hpp"
extern "C" {
#include "flute/flute.h"
}

using namespace std;
Matrix::Matrix( int n, int m){
    _matrix.resize(n, vector<bool>(m));
}

Matrix::Matrix( pair<int, int> size){
    int n = size.first;
    int m = size.second;
    Matrix(n, m);
}

Matrix::Matrix(){}

Circuit Matrix::circuit( int m){//TODO TEST
    readLUT();
    int x[MAXD];
    int y[MAXD];
    x[0] = 0;y[0] = 0;
    x[1] = 0;y[1] = 2;
    x[2] = 2;y[2] = 1;
    x[3] = 1;y[3] = 3;
    Tree fluteTree = flute(4,x,y,ACCURACY);
    printtree(fluteTree);

    Circuit circuit_l = Lwr_CNOT_Synth( m);
    transpose();
    Circuit circuit_u = Lwr_CNOT_Synth( m);
    circuit_u.switchCtrlTarget();
    circuit_l.reverse();                     // here we reverse circuit_l not circuit_u because our vector operation is push_back not push_front, which are the paper's operation
    return (circuit_u+=circuit_l);
}
Circuit Matrix::circuit(){
    
    return circuit( floor( log2(rowSize())));
}

void Matrix::transpose(){
    auto _backUpMatrix = _matrix;
    for( size_t i = 0; i < _matrix.size(); ++i ){
        for ( size_t j = 0; j < _matrix[i].size(); ++j ){
            _matrix[i][j] = _backUpMatrix[j][i];
        }
    }
    // cout << "Transpose" << endl;
    // cout << *this << endl;
}

Circuit Matrix::Lwr_CNOT_Synth( int m){
    Circuit circuit(rowSize());
    vector< vector< int> > columnSections((columnSize()/m), vector< int>(m));
    if(columnSize()%m)
        columnSections.push_back(vector< int>(columnSize()%m));
    for( size_t i= 0; i< columnSize(); ++i){
        columnSections[i/m][i%m] = i;
    }
    vector< int> patterns;
    for( size_t sec= 0; sec< columnSections.size(); ++sec){
        patterns.resize(pow(2, columnSections[sec].size()));
        for( auto& pattern: patterns){
            pattern = -1;
        }
        for( size_t row= columnSections[sec][0]; row < rowSize(); ++row){
            int subRowPattern = 0;
            for( size_t digit= 0; digit< columnSections[sec].size(); digit++){
                int column = columnSections[sec][digit];
                subRowPattern |= ((int)_matrix[row][column]) << digit;
            }
            if( subRowPattern == 0) continue;
            if( patterns[subRowPattern] == -1){
                patterns[subRowPattern] = row;
            }
            else{
                int fromRow = patterns[subRowPattern];
                int toRow = row;
                // cout << "Step A: add row " << fromRow << " to row " << toRow << endl;
                rowOperation(fromRow, toRow);
                circuit.push_back(make_pair(fromRow, toRow));
                // cout << *this << endl;
            }
        }
        for( auto& column: columnSections[sec]){
            bool diagOne = true;
            if( _matrix[column][column] == 0)
                diagOne = false;
            for( size_t row= column+1; row < rowSize(); ++row){
                if( _matrix[row][column] == true){
                    if ( diagOne == false){
                        int fromRow = row;
                        int toRow = column;
                        // cout << "Step B: Add row " << fromRow << " to row " << toRow << endl;
                        rowOperation(fromRow, toRow);
                        circuit.push_back(make_pair(fromRow, toRow));
                        // cout << *this << endl;
                        diagOne = true;
                    }
                    int fromRow = column;
                    int toRow = row;
                    // cout << "Step C: Add row " << fromRow << " to row " << toRow << endl;
                    rowOperation(fromRow, toRow);
                    circuit.push_back(make_pair(fromRow, toRow));
                    // cout << *this << endl;
                }
            }
        }

    }
    return circuit;
}

void Matrix::rowOperation( int from, int to){
    transform(_matrix[from].cbegin(), _matrix[from].cend(), _matrix[to].cbegin(), _matrix[to].begin(), [](bool a, bool b)->bool{return a^b;});
}

void Matrix::rowResize(int rowNum){
    _matrix.resize( rowNum);
}

void Matrix::columnResize(int ColumnNum){
    for( auto& row: _matrix){
        row.resize(ColumnNum);
    }
}

size_t Matrix::rowSize(){
    return _matrix.size();
}

size_t Matrix::columnSize(){
    return _matrix[0].size();
}

bool Matrix::operator==(const Matrix& m2){
    return _matrix == m2._matrix;
}

vector< bool>& Matrix::operator[](int index){
    return _matrix[index];
}

ostream& operator<< ( ostream& os, const Matrix& matrix){
    for( const auto& row: matrix._matrix){
        for ( const auto& element: row){
            os << element << ' ';
        }
        os << '\n';
    }
    return os;
}