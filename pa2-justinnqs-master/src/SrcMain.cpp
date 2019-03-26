#include "SrcMain.h"
#include <iostream>
#include <random>
#include "TSP.h"
#include <fstream>
#include <algorithm>

void ProcessCommandArgs(int argc, const char* argv[])
{
    std::string inputName = argv[1];
    int popSize = atoi(argv[2]);
    int generations = atoi(argv[3]);
    double mutThresh = static_cast<double>(atoi(argv[4])/100.0);
    int seed = atoi(argv[5]);
    
    std::vector<Location> locationVec = ParseLocations(inputName);
    std::mt19937 rand(seed);
    std::ofstream ofile("log.txt");
    Population pop = CreatePopulation(rand, popSize, locationVec.size());
    for (int i = 0; i <= generations; i++)
    {
        if(i == 0)
        {
            ofile << "INITIAL POPULATION:" << std::endl;
        }
        else
        {
            ofile << "GENERATION: " << i << std::endl;
        }
        WriteGeneration(ofile, pop);
        std::vector<std::pair<int, double>> fitVec = GetFitness(pop, locationVec);
        ofile << "FITNESS:" << std::endl;
        for (std::pair<int, double> i : fitVec)
        {
            ofile << i.first << ":" << i.second << std::endl;
        }
        std::vector<std::pair<int, double>> sortedVec = Sort(fitVec);
        
        std::vector<std::pair<int, int>> selectedVec = GetPairs(sortedVec, fitVec, sortedVec.size(), rand);
        if(i != generations)
        {
            ofile << "SELECTED PAIRS:" << std::endl;
            WriteSelectedPairs(ofile, selectedVec);
            Population newPopulation = CrossPopulation(selectedVec, pop, rand, mutThresh);
            pop = newPopulation;
        }
        if(i == generations)
        {
            ofile << "SOLUTION:" << std::endl;
            WriteSolution(ofile, locationVec, pop, sortedVec);
            ofile << locationVec[0].mName << std::endl;
            ofile << "DISTANCE: " << sortedVec[0].second << " miles";
        }
    }
    ofile.close();
}

