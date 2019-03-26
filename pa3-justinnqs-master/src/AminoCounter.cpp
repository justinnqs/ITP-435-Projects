#include "AminoCounter.h"

void Amino::Counter(std::string sequence)
{
    int input = -1, goTo = 0, total = 0;
    for(char& c : sequence)
    {
        if(c == 'T')
        {
            input = 0;
        }
        else if(c == 'C')
        {
            input = 1;
        }
        else if(c == 'A')
        {
            input = 2;
        }
        else if(c == 'G')
        {
            input = 3;
        }
        goTo = mStateMach[goTo][input];
        if(goTo > 23)
        {
            total++;
            auto it = mAminoCount.find(goTo);
            if(it != mAminoCount.end())
            {
                it->second++;
            }
            goTo = 3;
        }
    }
    mTotalAmino = total;
}

std::unordered_map<int, int> Amino::GetAminoCount()
{
    return mAminoCount;
}

int Amino::GetSpecAmino(int aminoID)
{
    auto it = mAminoCount.find(aminoID);
    if(it != mAminoCount.end())
    {
        return it->second;
    }
    
    return 0;
}

int Amino::GetTotal()
{
    return mTotalAmino;
}
