#ifndef GENERATE_HPP
#define GENERATE_HPP

#include<iostream>
#include<fstream>
#include<random>
#include<vector>
#include<unordered_map>
#include<boost/filesystem.hpp>
#include"cGraph.hpp"

using namespace std;

void Cgenerate(string fileName, int InputNum, int GateNum);

void Cgenerate(string fileName, int InputNum, int GateNum, CGraph& );

void Tgenerate(string fileName, int InputNum);

void Ggenerate(string fileName, int graphWidth);

#endif