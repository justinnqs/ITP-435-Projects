#include "PairWise.h"
#include "ParseFile.h"

SeqCompare::SeqCompare(std::string inFile1, std::string inFile2)
{
    Parser parser1(inFile1);
    Parser parser2(inFile2);
    mSequence1 = parser1.GetSequence();
    mSequence2 = parser2.GetSequence();
    mHeader1 = parser1.GetHeader();
    mHeader2 = parser2.GetHeader();
}

void SeqCompare::CompAlg()
{
    std::vector<short> tempScore;
    std::vector<char> tempDir;
    int i = -1;
    tempScore.push_back(0);
    for(char c : mSequence1)
    {
        tempScore.push_back(i);
        tempDir.push_back(Direction::left);
        i--;
    }
    mGridScore.push_back(tempScore);
    mGridDir.push_back(tempDir);
    tempDir.clear();
    tempDir.push_back(above);
    for(int i = 1; i <= mSequence2.size(); i++)
    {
        tempScore.clear();
        tempScore.push_back(0 - i);
        mGridScore.push_back(tempScore);
        mGridDir.push_back(tempDir);
    }
    
    for(int i = 1; i < mSequence2.size()+1; i++)
    {
        short tieBreak;
        for(int j = 1; j < mSequence1.size()+1; j++)
        {
            std::pair<short, char> cell = CalcCell(i, j, tieBreak);
            mGridScore[i].push_back(cell.first);
            mGridDir[i].push_back(cell.second);
        }
    }
}

std::pair<short, char> SeqCompare::CalcCell(int i, int j, short& tieBreak)
{
    short aboveLeft = 0, above = 0, left = 0;
    // aboveLeft calc
    if(mSequence1.at(j-1) == mSequence2.at(i-1))
    {
        aboveLeft = mMatch + mGridScore[i-1][j-1];
    }
    else
    {
        aboveLeft = mMismatch + mGridScore[i-1][j-1];
    }
    
    left = mGridScore[i][j-1] + mGap;
    above = mGridScore[i-1][j] + mGap;
    short max = GetMax(aboveLeft, left, above);
    if(IsTie(aboveLeft, left, above) == max)
    {
        if(aboveLeft == max)
        {
            std::pair<short, char> cell = std::make_pair(max, Direction::aboveLeft);
            return cell;
        }
        else if(left == max)
        {
            std::pair<short, char> cell = std::make_pair(max, Direction::left);
            return cell;
        }
        else if(above == max)
        {
            std::pair<short, char> cell = std::make_pair(max, Direction::above);
            return cell;
        }
    }
    else
    {
        if(max == aboveLeft)
        {
            std::pair<short, char> cell = std::make_pair(max, Direction::aboveLeft);
            return cell;
        }
        else if(max == left)
        {
            std::pair<short, char> cell = std::make_pair(max, Direction::left);
            return cell;
        }
        else if(max == above)
        {
            std::pair<short, char> cell = std::make_pair(max, Direction::above);
            return cell;
        }
    }
    std::pair<short, char> cell = std::make_pair(0, 'N');
    return cell;
}

short SeqCompare::GetMax(int x, int y, int z)
{
    int max = (x < y) ? y : x;
    return ((max < z) ? z : max);
}

short SeqCompare::IsTie(int x, int y, int z)
{
    if(x == y || x == z)
    {
        return x;
    }
    else if(y == z)
    {
        return y;
    }
    return -1;
}

void SeqCompare::ConstructOutput()
{
    int maxSize = mSequence2.size();
    
    if(mSequence1.size() > mSequence2.size())
    {
        maxSize = mSequence1.size();
    }
    mOutput1.reserve(maxSize);
    mOutput2.reserve(maxSize);
    int i = mGridDir.size()-1;
    int j = mGridDir[i].size()-1;
    
    for(int k = 0; k < maxSize; k++)
    {
        if(mGridDir[i][j] == 'k')
        {
            mOutput1 += mSequence1[j-1];
            mOutput2 += mSequence2[i-1];
            i -= 1;
            j -= 1;
        }
        else if(mGridDir[i][j] == 'l')
        {
            mOutput1 += mSequence1[j-1];
            mOutput2 += '_';
            j -= 1;
        }
        else if(mGridDir[i][j] == 'a')
        {
            mOutput1 += '_';
            mOutput2 += mSequence2[i-1];
            i -= 1;
        }
    }
    std::reverse(mOutput1.begin(), mOutput1.end());
    std::reverse(mOutput2.begin(), mOutput2.end());
    
}

void SeqCompare::Output()
{
    std::ofstream ofile("match.result");
    ofile << "A: " << mHeader1 << std::endl;
    ofile << "B: " << mHeader2 << std::endl;
    ofile << "Score: " << mGridScore[mGridScore.size()-1][mGridScore[0].size()-1] << std::endl;
    ofile << std::endl;
    for(int i = 0; i < mOutput1.size(); i++)
    {
        for(int j = 0; j < 70; j++)
        {
            if(i+j < mOutput1.size())
            {
                ofile << mOutput1[i+j];
            }
        }
        ofile << std::endl;
        for(int j = 0; j < 70; j++)
        {
            if(mOutput1[i+j] == mOutput2[i+j])
            {
                if(i+j < mOutput1.size())
                {
                    ofile << "|";
                }
            }
            else
            {
                if(i+j < mOutput1.size())
                {
                    ofile << " ";
                }
            }
        }
        ofile << std::endl;
        for(int j = 0; j < 70; j++)
        {
            if(i+j < mOutput1.size())
            {
                ofile << mOutput2[i+j];
            }
            else
            {
                ofile << std::endl;
                return;
            }
        }
        ofile << std::endl;
        i += 69;
    }
    ofile.close();
}

