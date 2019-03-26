#include "SrcMain.h"
#include "ParseFile.h"
#include "AminoCounter.h"
#include "PairWise.h"
#include <iostream>

void ProcessCommandArgs(int argc, const char* argv[])
{
    // name of the FASTA file for amino acid count
	if(argc == 2)
    {
        std::string inFile = argv[1];
        Parser parser(inFile);
        Amino aminoCount;
        aminoCount.Counter(parser.GetSequence());
        parser.OutputAmino(aminoCount, "amino.txt");
    }
    // name of the two FASTA files for pairwise
    else if (argc == 3)
    {
        std::string inFile1 = argv[1];
        std::string inFile2 = argv[2];
        SeqCompare pairWise(inFile1, inFile2);
        pairWise.CompAlg();
        pairWise.ConstructOutput();
        pairWise.Output();
    }
}
