#include "RleFile.h"
#include "RleData.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>

void RleFile::CreateArchive(const std::string& source)
{
    std::ifstream::pos_type size;
    char* memblock;
    // Open the file for input, in binary mode, and start ATE (at the end)
    std::ifstream file (source, std::ios::in|std::ios::binary|std::ios::ate);
    if (file.is_open())
    {
        size = int(file.tellg()); // Save the size of the file
        memblock = new char[static_cast<unsigned int>(size)];
        file.seekg(0, std::ios::beg); // Seek back to start of file
        file.read(memblock, size);
        file.close();
        
        mRleData.Compress(memblock, size);
        mHeader.mFileName = source;
        mHeader.mFileNameLength = static_cast<unsigned char>(mHeader.mFileName.length());
        mHeader.mFileSize = static_cast<int>(size);
        std::ofstream arc(mHeader.mFileName + ".rl1", std::ios::out|std::ios::binary|std::ios::trunc);
        if (arc.is_open())
        {
            arc.write(mHeader.mSig, 4);
            arc.write(reinterpret_cast<char*>(&(mHeader.mFileSize)), 4);
            arc.write(reinterpret_cast<char*>(&(mHeader.mFileNameLength)), 1);
            arc.write(mHeader.mFileName.c_str(), static_cast<int>(mHeader.mFileNameLength));
            arc.write(mRleData.mData, mRleData.mSize);
            
            arc.close();
        }
        
        delete[] memblock;
    }
}

void RleFile::ExtractArchive(const std::string& source)
{
    std::ifstream::pos_type size;
    char* memblock;
    // Open the file for input, in binary mode, and start ATE (at the end)
    std::ifstream file (source, std::ios::in|std::ios::binary|std::ios::ate);
    if (file.is_open())
    {
        size = file.tellg(); // Save the size of the file
        memblock = new char[static_cast<unsigned int>(size)];
        file.seekg(0, std::ios::beg); // Seek back to start of file
        file.read(memblock, static_cast<int>(size));
        file.close();

        mHeader.mFileSize = *(reinterpret_cast<int*>(&memblock[4]));
        mHeader.mFileNameLength = (memblock[8]);
        for (unsigned char i = 0; i < mHeader.mFileNameLength; i++)
        {
            mHeader.mFileName += memblock[9 + i];
        }

        mRleData.Decompress(&memblock[mHeader.mFileNameLength + 9], static_cast<size_t>(size) - (mHeader.mFileNameLength + 9), mHeader.mFileSize);

        std::ofstream arc(mHeader.mFileName, std::ios::out|std::ios::binary|std::ios::trunc);
        if (arc.is_open())
        {
            arc.write(mRleData.mData, mRleData.mSize);
            arc.close();
        }

        delete[] memblock;
    }
}
