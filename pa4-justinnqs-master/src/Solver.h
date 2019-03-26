#pragma once

#include "sha1.h"
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <unordered_map>
#include <map>
#include <iterator>
#include <tbb/parallel_invoke.h>

class Solver
{
public:
    void ParseDictionary(const std::string& inFile);
    void ParsePassword(const std::string& inFile);
    std::string GetHash(const std::string& input);
    void LookUp();
    void InitBruteForce();
    void ParBruteForce();
    void BruteForce(std::vector<unsigned int> start, std::vector<unsigned int> end);
//    std::string BruteForce(std::string input, std::vector<unsigned int> start, std::vector<unsigned int> end);
    std::string DecodeCounter(std::vector<unsigned int> input);
    void Output();
    std::vector<std::string> GetmPassDecr();
private:
    std::unordered_map<std::string, std::string> mDictionary;
    std::unordered_map<std::string, std::string> mBFDictionary;
    std::vector<std::string> mPassHash;
    std::vector<std::string> mPassDecr;
    char mDecoder[36] = {
        'a', 'b', 'c', 'd',
        'e', 'f', 'g', 'h',
        'i', 'j', 'k', 'l',
        'm', 'n', 'o', 'p',
        'q', 'r', 's', 't',
        'u', 'v', 'w', 'x',
        'y', 'z', '0', '1',
        '2', '3', '4', '5',
        '6', '7', '8', '9',
    };
    bool mDoBrute = false;
};

