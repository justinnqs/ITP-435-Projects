#include "SrcMain.h"
#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include "Node.h"
#include <fstream>

extern int gLineNumber;
extern NBlock* gMainBlock;

extern int zompilerparse(); // NOLINT
struct yy_buffer_state; // NOLINT
extern void zompiler_flush_buffer(yy_buffer_state* b); // NOLINT
extern FILE* zompilerin; // NOLINT

void ProcessCommandArgs(int argc, const char* argv[])
{
	if (argc < 2)
	{
		std::cout << "You must pass the input file as a command line parameter." << std::endl;
		return;
	}

	// Read input from the first command line parameter
	zompilerin = fopen(argv[1], "r");
	if (zompilerin == nullptr)
	{
		std::cout << "File not found: " << argv[1] << std::endl;
		return;
	}
	zompiler_flush_buffer(nullptr);

	// Start the parse
	zompilerparse();

	// TODO: CodeGen from gMainBlock
    CodeContext codeContext;
    if(gMainBlock != nullptr)
    {
        if(argc == 3 && argv[2] == "-o")
        {
            gMainBlock->Optimize();
        }
        gMainBlock->CodeGen(codeContext);
    }
    std::ofstream ofile;
    ofile.open("out.zom");
    for(std::string output : codeContext.mOps)
    {
        ofile << output << std::endl;
    }
	// Close the file stream
	fclose(zompilerin);
}

void zompilererror(const char* s) // NOLINT
{
	std::cout << s << " on line " << gLineNumber << std::endl;
}
