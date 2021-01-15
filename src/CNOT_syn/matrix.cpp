#include"matrix.hpp"

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

Circuit Matrix::circuit( int m){
    Circuit circuit_l = Lwr_CNOT_Synth( m, NULL, false);
    transpose();
    Circuit circuit_u = Lwr_CNOT_Synth( m, NULL, true);
    circuit_u.switchCtrlTarget();
    circuit_l.reverse();                     // here we reverse circuit_l not circuit_u because our vector operation is push_back not push_front, which are the paper's operation
    return (circuit_u+=circuit_l);
}

Circuit Matrix::circuit(){
    int m = floor( log2(rowSize()));
    return circuit( m);
}

Circuit Matrix::circuit( CGraph* cgraph){
    if( cgraph != NULL ){
        readLUT();
    }
    Circuit circuit_l = Lwr_CNOT_Synth( 1, cgraph, false);
    transpose();
    Circuit circuit_u = Lwr_CNOT_Synth( 1, cgraph, true);
    circuit_u.switchCtrlTarget();
    circuit_l.reverse();                     // here we reverse circuit_l not circuit_u because our vector operation is push_back not push_front, which are the paper's operation
    return (circuit_u+=circuit_l);
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

void Matrix::totalReverse(){
    reverse(_matrix.begin(), _matrix.end());
    for( auto& row:_matrix){
        reverse(row.begin(), row.end());
    }
}

Circuit Matrix::Lwr_CNOT_Synth( int m, CGraph* cgraph=NULL, bool u=false){
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
        int ctrl = -1;
        vector< int> targets;

        if ( cgraph == NULL){
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
                    // rowOperation(fromRow, toRow);
                    // circuit.push_back(make_pair(fromRow, toRow));
                    // cout << *this << endl;
                    if ( ctrl == -1) ctrl = fromRow;
                    targets.push_back(toRow);
                }
            }
            for( auto& target: targets){
                    // cout << "Step A: add row " << fromRow << " to row " << toRow << endl;
                    rowOperation(ctrl, target);
                    circuit.push_back(make_pair(ctrl, target));
                    // cout << *this << endl;
            }

            ctrl = -1;
            targets.clear();
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
                        if( cgraph == NULL){
                            // cout << "Step B: Add row " << fromRow << " to row " << toRow << endl;
                            rowOperation(fromRow, toRow);
                            circuit.push_back(make_pair(fromRow, toRow));
                        }
                        else{
                            vector< int> singleTarget = {toRow};
                            vector< pair< int, int> >& rowOperations = cgraph->rowOperations(fromRow, singleTarget, u);
                            for( const auto&[ctrl, target]: rowOperations){
                                // cout << "Step B: add row " << ctrl << " to row " << target<< endl;
                                rowOperation(ctrl, target);
                                circuit.push_back(make_pair(ctrl, target));
                                // cout << *this << endl;
                            }
                            cout << "step B" << endl;
                            cout << *this << endl;
                        }
                        diagOne = true;
                    }
                    int fromRow = column;
                    int toRow = row;
                    // cout << "Step C: Add row " << fromRow << " to row " << toRow << endl;
                    // rowOperation(fromRow, toRow);
                    // circuit.push_back(make_pair(fromRow, toRow));
                    // cout << *this << endl;
                    if ( ctrl == -1) ctrl = fromRow;
                    targets.push_back(toRow);
                }
            }
        }
        if( cgraph == NULL){
            for( auto& target: targets){
                    // cout << "Step C: Add row " << ctrl << " to row " << target << endl;
                    rowOperation(ctrl, target);
                    circuit.push_back(make_pair(ctrl, target));
                    // cout << *this << endl;
            }
        }
        else{
            //TODO
            vector< pair< int, int> >& rowOperations = cgraph->rowOperations(ctrl, targets, u);
            for( const auto&[ctrl, target]: rowOperations){
                    // cout << "Step C: add row " << ctrl << " to row " << target<< endl;
                    rowOperation(ctrl, target);
                    circuit.push_back(make_pair(ctrl, target));
                    // cout << *this << endl;
            }
            cout << "step C" << endl;
            cout << *this << endl;
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

istream& operator>> ( istream& is, Matrix& matrix){
    string header;
    getline(is, header);
    string delimiter = "=";
    int n = std::stoi(header.substr(header.find(delimiter)+1, header.length()));
    matrix.rowResize(n);
    matrix.columnResize(n);
    string Row;
    int rowCount = 0;
    int columnCount = 0;
    delimiter = " ";
    while(getline(is, Row)){
        stringstream iss( Row );
        bool element;
        while ( iss >> element ){
        matrix._matrix[rowCount][columnCount] = element;
        ++columnCount;
        if( columnCount == n) columnCount =0;
        }
        ++rowCount;
    }
    return is;
}