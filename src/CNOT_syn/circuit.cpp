#include"circuit.hpp"

using namespace std;
Circuit::Circuit(){}
Circuit::Circuit( int i):inputNum(i){}
void Circuit::switchCtrlTarget(){
    for( auto& CNotGate:_circuit){
        swap(CNotGate.first, CNotGate.second);
    }
}

void Circuit::reverse(){
    std::reverse(_circuit.begin(), _circuit.end());
}

void Circuit::push_back(pair< int, int> CNotGate){
    _circuit.push_back(CNotGate);
}

Circuit& Circuit::operator+=(const Circuit& c2){
    _circuit.insert(_circuit.end(), c2._circuit.begin(), c2._circuit.end());
    return *this;
}

bool Circuit::operator==( const Circuit& c2){
    return _circuit == c2._circuit;
}
Matrix Circuit::matrix(){
    Matrix _matrix;
    _matrix.rowResize(inputNum);
    _matrix.columnResize(inputNum);
    for(size_t i = 0; i < _matrix.rowSize(); ++i){
        _matrix[i][i] = 1;
    }

    for( auto& [ctrlGate, targetGate]: _circuit){
        for( size_t element = 0; element < _matrix[ctrlGate].size(); ++element){
            if ( _matrix[ctrlGate][element] == false )continue;
            _matrix[targetGate][element] = _matrix[targetGate][element]^1;
        }
    }
    
    return _matrix;
}

void Circuit::setInputNum( int i){
    inputNum = i;
}

int Circuit::getInputNum(){
    return inputNum;
}

int Circuit::getCircuitSize(){
    return _circuit.size();
}

template< typename T1, typename T2>
ostream& operator<< ( ostream& os, const pair< T1, T2>& _pair){
    os << _pair.first << ' ' << _pair.second;
    return os;
}

ostream& operator<< ( ostream& os, const Circuit& circuit){
    os << "InputNum = " << circuit.inputNum; 
    for( const auto& CNOTGate: circuit._circuit){
        os << '\n' << CNOTGate;
    }
    return os;
}

istream& operator>> ( istream& is, Circuit& circuit){
    string header;
    getline(is, header);
    string delimiter = "=";
    circuit.inputNum = std::stoi(header.substr(header.find(delimiter)+1, header.length()));
    string CNOTGate;
    int ctrlGate;
    int targetGate;
    delimiter = " ";
    while(getline(is, CNOTGate)){
        ctrlGate = std::stoi(CNOTGate.substr(0, CNOTGate.find(delimiter)));
        targetGate = std::stoi(CNOTGate.substr(CNOTGate.find(delimiter), CNOTGate.length()));
        circuit.push_back(make_pair( ctrlGate, targetGate) );
    }
    return is;
}


