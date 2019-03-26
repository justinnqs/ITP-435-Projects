#include "Solver.h"

void Solver::ParseDictionary(const std::string& inFile)
{
    std::ifstream file (inFile);
    if(file.is_open())
    {
        std::string fileLine;
        file.seekg(0, std::ios::beg); // Seek back to start of file
        while(static_cast<bool>(std::getline(file, fileLine)))
        {
            mDictionary.insert(make_pair(GetHash(fileLine), fileLine));
        }
    }
}

void Solver::ParsePassword(const std::string& inFile)
{
    std::ifstream file (inFile);
    if(file.is_open())
    {
        std::string fileLine;
        while(static_cast<bool>(std::getline(file, fileLine)))
        {
            mPassHash.emplace_back(fileLine);
            mPassDecr.emplace_back("??");
            mBFDictionary.insert(std::make_pair(fileLine, "??"));
        }
    }
}

std::string Solver::GetHash(const std::string& input)
{
    unsigned char hash[20];
    sha1::Calc(input.c_str(), input.size(), hash);
    char hexStr[41];
    sha1::ToHexString(hash, hexStr);
    return hexStr;
}

void Solver::LookUp()
{
    for(int i = 0; i < mPassHash.size(); i++)
    {
        auto search = mDictionary.find(mPassHash[i]);
        if(search != mDictionary.end())
        {
            mPassDecr[i] = search->second;
            auto search = mBFDictionary.find(mPassHash[i]);
            if(search != mBFDictionary.end())
            {
                mBFDictionary.erase(mBFDictionary.find(mPassHash[i]));
            }
        }
    }
    for(int i = 0; i < mPassHash.size(); i++)
    {
        if(mPassDecr[i] == "??")
        {
            mDoBrute = true;
            break;
        }
    }
}

void Solver::ParBruteForce()
{
    if(mDoBrute)
    {
        tbb::parallel_invoke(
            [this] { BruteForce({0}, {35}); },
            [this] { BruteForce({0,0}, {35,35}); },
            [this] { BruteForce({0,0,0}, {11,35,35}); },
            [this] { BruteForce({12,0,0}, {23,35,35}); },
            [this] { BruteForce({24,0,0}, {35,35,35}); },
            [this] { BruteForce({0,0,0,0}, {8,35,35,35});},
            [this] { BruteForce({9,0,0,0}, {17, 35, 35, 35});},
            [this] { BruteForce({18,0,0,0}, {26,35,35,35});},
            [this] { BruteForce({27,0,0,0}, {35, 35, 35, 35});}
        );
        for(int i = 0; i < mPassHash.size(); i++)
        {
            if(mPassDecr[i] == "??")
            {
                auto search = mBFDictionary.find(mPassHash[i]);
                if(search != mBFDictionary.end())
                {
                    mPassDecr[i] = search->second;
                }
                else
                {
                    mPassDecr[i] = "??";
                }
            }
        }
    }
}

void Solver::BruteForce(std::vector<unsigned int> start, std::vector<unsigned int> end)
{
    std::vector<unsigned int> counter = start;
    bool isFinished = false;
    while(!isFinished)
    {
        auto search = mBFDictionary.find(GetHash(DecodeCounter(counter)));
        if(search != mDictionary.end())
        {
            search->second = DecodeCounter(counter);
        }
        if(std::equal(counter.begin(), counter.end(), end.begin()))
        {
            break;
        }
        counter[0]++;
        if(counter[0] == 36)
        {
            counter[0] = 0;
            counter[1] += 1;
        }
        if(counter[1] == 36)
        {
            counter[1] = 0;
            counter[2] += 1;
        }
        if(counter[2] == 36)
        {
            counter[2] = 0;
            counter[3] += 1;
        }
    }
}

std::string Solver::DecodeCounter(std::vector<unsigned int> input)
{
    std::string output;
    for(auto a: input)
    {
        output += mDecoder[a];
    }
    
    return output;
}

void Solver::Output()
{
    std::ofstream ofile("solved.txt");
    for(int i = 0; i < mPassHash.size(); i++)
    {
        ofile << mPassHash[i] << "," << mPassDecr[i] << std::endl;
    }
}

std::vector<std::string> Solver::GetmPassDecr()
{
    return mPassDecr;
}
