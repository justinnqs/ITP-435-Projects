#pragma once
#include "AminoCounter.h"
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <unordered_map>

class Parser
{
public:
    Parser(std::string& inFile);
    std::string GetHeader();
    std::string GetSequence();
    void OutputAmino(Amino amino, std::string outFile);
private:
    std::string mHeader;
    std::string mSequence;
};
