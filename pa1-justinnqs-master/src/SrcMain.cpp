#include "SrcMain.h"
#include <string>
#include "RleFile.h"
#include <iostream>
#include <cstdio>
#include <string>
void ProcessCommandArgs(int argc, const char* argv[])
{
	// TODO: Process argc and argv, and use to compress/decompress files
    std::string str = argv[1];
    RleFile r;

    if (str.substr(str.size() - 3, str.size())=="rl1")
    {
        r.ExtractArchive(str);
    }
    else
    {
        r.CreateArchive(str);
    }
//    if (str.find(".rl1"))
//    {
//        r.ExtractArchive(str);
//    }
//    else
//    {
//        r.CreateArchive(str);
//    }
}

