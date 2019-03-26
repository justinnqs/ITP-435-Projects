#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <utility>

class SeqCompare
{
public:
    SeqCompare(std::string inFile1, std::string inFile2);
    void CompAlg();
    std::pair<short, char> CalcCell(int i, int j, short& tieBreak);
    short GetMax(int x, int y, int z);
    short IsTie(int x, int y, int max);
    void ConstructOutput();
    void Output();
private:
    std::string mHeader1;
    std::string mHeader2;
    std::string mSequence1;
    std::string mSequence2;
    std::vector<std::vector<short>> mGridScore;
    std::vector<std::vector<char>> mGridDir;
    short mMatch = 1;
    short mMismatch = -1;
    short mGap = -1;
    std::string mOutput1;
    std::string mOutput2;
    enum Direction : char
    {
        aboveLeft = 'k',
        left = 'l',
        above = 'a'
    };
};
