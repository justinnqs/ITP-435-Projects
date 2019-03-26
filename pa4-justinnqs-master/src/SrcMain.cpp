#include "SrcMain.h"
#include "Solver.h"

void ProcessCommandArgs(int argc, const char* argv[])
{
	if(argc == 3)
    {
        Solver solver;
        solver.ParseDictionary(argv[1]);
        solver.ParsePassword(argv[2]);
        solver.LookUp();
        solver.ParBruteForce();
        solver.Output();
    }
}
