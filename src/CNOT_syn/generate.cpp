#include"generate.hpp"

using namespace std;

void Cgenerate(string fileName, int InputNum, int GateNum){
    ofstream file(fileName+".circuit");
    file << "InputNum = " << InputNum << endl;
    static random_device rd;
    static auto gen = default_random_engine(rd());
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

void Cgenerate(string fileName, int InputNum, int GateNum, CGraph& cg){
    ofstream file(fileName+".circuit");
    file << "InputNum = " << InputNum << endl;
    static random_device rd;
    static auto gen = default_random_engine(rd());
    uniform_int_distribution<int> dis(0, InputNum-1);
    for( int i = 0; i < GateNum; ++i){
        int ctrlGate = dis(gen);
        auto& qb = cg.getQubit(ctrlGate);
        int Nsize = qb.getneighbors().size();
        uniform_int_distribution<int> _dis(0, Nsize-1);
        int notGate = _dis(gen);
        notGate = qb.getneighbors()[notGate];
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

void Ggenerate(string fileName, int graphWidth){
    ofstream file(fileName+".cgraph");
    unordered_map< pair<int, int>, int, pair_hash> map;
    file << "n = " << (int)pow(graphWidth, 2) << '\n';
    for( int i = 0, x = 0, y = 0; i < pow(graphWidth, 2); ++i){
        x = i%graphWidth;
        y = i/graphWidth;
        map.insert({{x,y}, i});
    }
    for( int i = 0, x = 0, y = 0; i < pow(graphWidth, 2); ++i){
        x = i%graphWidth;
        y = i/graphWidth;
        vector<int> neighbors;
        vector<pair<int, int>> directions = {{0,1}, {1,0}, {0,-1}, {-1,0}};
        for( pair<int, int> direction: directions){
            int _x = x+direction.first;
            int _y = y+direction.second;
            if( map.find({_x, _y}) != map.end()){
                neighbors.push_back(map[{_x,_y}]);
            }
        }
        file << i << " (" << x << ',' << y << ") {";
        for( auto& neighbor: neighbors){
            file << neighbor;
            if( neighbor != *neighbors.rbegin()) file << ',';
        }
        file << "}\n";
    }
    
}