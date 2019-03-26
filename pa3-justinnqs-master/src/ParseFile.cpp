#include "ParseFile.h"

Parser::Parser(std::string& inFile)
{
    std::ifstream::pos_type size;
    std::ifstream file (inFile, std::ios::in|std::ios::ate);
    if(file.is_open())
    {
        char parseChar;
        size = file.tellg();
        mSequence.reserve(size);
        file.seekg(0, std::ios::beg); // Seek back to start of file
        file.get(parseChar);
        std::getline(file, mHeader);
//        file.get(parseChar);
        while(static_cast<bool>(file.get(parseChar)))
        {
            if(parseChar == 'A' || parseChar == 'T' || parseChar == 'C' || parseChar == 'G')
            {
                mSequence += parseChar;
            }
        }
    }
}

std::string Parser::GetHeader()
{
    return mHeader;
}

std::string Parser::GetSequence()
{
    return mSequence;
}

void Parser::OutputAmino(Amino amino, std::string outFile)
{
    std::ofstream ofile(outFile);
    ofile << mHeader << std::endl;
    ofile << "Total amino acids produced: " << amino.GetTotal() << std::endl;
    std::unordered_map<int, int> temp = amino.GetAminoCount();
    ofile << "(A) Alanine: " << amino.GetSpecAmino(24) << std::endl;
    ofile << "(C) Cysteine: " << amino.GetSpecAmino(25) << std::endl;
    ofile << "(D) Aspartic acid: " << amino.GetSpecAmino(26) << std::endl;
    ofile << "(E) Glutamic acid: " << amino.GetSpecAmino(27) << std::endl;
    ofile << "(F) Phenylalanine: " << amino.GetSpecAmino(28) << std::endl;
    ofile << "(G) Glycine: " << amino.GetSpecAmino(29) << std::endl;
    ofile << "(H) Histidine: " << amino.GetSpecAmino(30) << std::endl;
    ofile << "(I) Isoleucine: " << amino.GetSpecAmino(31) << std::endl;
    ofile << "(K) Lysine: " << amino.GetSpecAmino(32) << std::endl;
    ofile << "(L) Leucine: " << amino.GetSpecAmino(33) << std::endl;
    ofile << "(M) Methionine: " << amino.GetSpecAmino(34) << std::endl;
    ofile << "(N) Asparagine: " << amino.GetSpecAmino(35) << std::endl;
    ofile << "(P) Proline: " << amino.GetSpecAmino(36) << std::endl;
    ofile << "(Q) Glutamine: " << amino.GetSpecAmino(37) << std::endl;
    ofile << "(R) Arginine: " << amino.GetSpecAmino(38) << std::endl;
    ofile << "(S) Serine: " << amino.GetSpecAmino(39) << std::endl;
    ofile << "(T) Threonine: " << amino.GetSpecAmino(40) << std::endl;
    ofile << "(V) Valine: " << amino.GetSpecAmino(41) << std::endl;
    ofile << "(W) Tryptophan: " << amino.GetSpecAmino(42) << std::endl;
    ofile << "(Y) Tyrosine: " << amino.GetSpecAmino(43) << std::endl;
    ofile.close();
}
