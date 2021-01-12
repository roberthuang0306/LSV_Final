#include"generate.hpp"

using namespace std;

void Cgenerate(string fileName, int InputNum, int GateNum){
    ofstream file(fileName+".circuit");
    file << "InputNum = " << InputNum << endl;
    random_device rd;
    auto gen = default_random_engine(rd());
    uniform_int_distribution<int> dis(0, InputNum-1);
    for( int i = 0; i < GateNum; ++i){
        int ctrlGate = dis(gen);
        int notGate = dis(gen);
        while(notGate == ctrlGate){
            notGate = dis(gen);
        }
        file << ctrlGate << ' ' << notGate << '\n';
    }
}

void Tgenerate(string fileName, int InputNum){ //not guarantee linear
    ofstream file(fileName+".table");
    random_device rd;
    auto gen = default_random_engine(rd());
    vector<int> outputs(pow(2,InputNum));
    for( int i = 0 ; i < pow(2,InputNum); ++i) outputs[i] = i;
    for( int input = 0 ; input < pow(2,InputNum); ++input){
        uniform_int_distribution<int> dis(0, outputs.size()-1);
        int outputIndex = dis(gen);
        int output = outputs[outputIndex];
        file << input << ' ' << output << '\n';
        swap(outputs[outputIndex], outputs[outputs.size()-1]);
        outputs.pop_back();
    }
}