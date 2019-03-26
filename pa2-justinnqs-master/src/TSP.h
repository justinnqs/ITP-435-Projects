#pragma once
#include <string>
#include <vector>
#include <random>

struct Location
{
    std::string mName = "";
    double mLatitude = 0.0;
    double mLongitude = 0.0;
};

struct Population
{
    std::vector<std::vector<int>> mMembers;
};

std::vector<Location> ParseLocations(std::string& filename);

Population CreatePopulation(std::mt19937& rand, int popSize, int numLocations);

std::vector<std::pair<int, double>> GetFitness(Population& pop, std::vector<Location>& locVec);

double CalcHaversine(Location loc1, Location loc2);

std::vector<std::pair<int, double>> Sort(std::vector<std::pair<int, double>> fitVec);

std::vector<double> GetProb(int size, std::vector<std::pair<int, double>>& fitVec);

std::vector<std::pair<int, int>> GetPairs(std::vector<std::pair<int, double>>& sortedVec, std::vector<std::pair<int, double>>& fitVec, int popSize, std::mt19937& rand);

Population CrossPopulation(std::vector<std::pair<int, int>>& selectedPairs, Population& pop, std::mt19937& rand, double mutationThresh);

void WriteGeneration(std::ofstream& ofile, Population pop);

void WriteSelectedPairs(std::ofstream& ofile, std::vector<std::pair<int, int>> selectedVec);

void WriteSolution(std::ofstream& ofile, std::vector<Location> locationVec, Population pop, std::vector<std::pair<int, double>> sortedVec);
