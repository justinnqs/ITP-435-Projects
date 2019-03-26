#include "RleData.h"
#include <iostream>
#include <vector>

void RleData::Compress(const char* input, size_t inSize)
{
    delete[] mData;
    mSize = 0;
    mData = new char[inSize*2];
    int runSize = 0;
    for(int i = 0; i < inSize; i++)
    {
        if(IsPositiveRun(input, i, inSize))
        {
            char target = input[i];
            for(int j = i; j < inSize; j++)
            {
                if(target == input[j])
                {
                    runSize++;
                    if(runSize > 126)
                    {
                        break;
                    }
                }
                else
                {
                    break;
                }
            }
            mData[mSize] = runSize;
            mSize++;
            mData[mSize] = target;
            mSize++;
            i += runSize - 1;
            runSize = 0;
        }
        else
        {
            char target = input[i];
            for(int j = i; j < inSize; j++)
            {
                if(j+1 < inSize)
                {
                    if(target != input[j+1])
                    {
                        runSize--;
                        if(runSize < -126)
                        {
                            break;
                        }
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    runSize--;
                    break;
                }
                target = input[j+1];
            }
            mData[mSize] = runSize;
            mSize++;
            for(int j = i; j < (i + abs(runSize)); j++)
            {
                mData[mSize] = input[j];
                mSize++;
            }
            i += abs(runSize) - 1;
            runSize = 0;

        }
    }
}

bool RleData::IsPositiveRun(const char* input, int index, size_t inSize)
{
    if(index-1 > 0)
    {
        if(input[index-1] == input[index])
        {
            return true;
        }
    }
    if (index+1 < inSize && index+2 < inSize)
    {
        if(input[index+1] == input[index+2])
        {
            return true;
        }
    }
    if(index+1 < inSize)
    {
        if(input[index] == input[index+1])
        {
            return true;
        }
    }
    return false;
}

void RleData::Decompress(const char* input, size_t inSize, size_t outSize)
{
    delete[] mData;
    mSize = 0;
    mData = new char[outSize];
//    std::string str = input;
    
    for(int i = 0; i < inSize; i++)
    {
        if(input[i] > 0)
        {
            for(int j = 0; j < input[i]; j++)
            {
                mData[mSize] = input[i+1];
                mSize++;
            }
            i += 1;
        }
        else if(input[i] < 0)
        {
            for(int j = i+1; j <= i + abs(input[i]); j++)
            {
                mData[mSize] = input[j];
                mSize++;
            }
            i+= abs(input[i]);
        }
    }
}

std::ostream& operator<< (std::ostream& stream, const RleData& rhs)
{
	for (int i = 0; i < rhs.mSize; i++)
	{
		stream << rhs.mData[i];
	}
	return stream;
}
