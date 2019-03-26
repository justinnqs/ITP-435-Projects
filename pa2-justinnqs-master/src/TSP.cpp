#include "TSP.h"
#include <fstream>
#include <algorithm>
#include <cmath>
#include <iterator>
#include <numeric>
#include <iostream>
#include <sstream>

std::vector<Location> ParseLocations(std::string& filename)
{
    std::ifstream inputFile(filename);
    
    std::vector<Location> locations;
    
    std::string line;
    if(inputFile.is_open())
    {
        while(static_cast<bool>(std::getline(inputFile, line)))
        {
            Location tempLocation = Location();
            std::vector<std::string> lineData;
            std::istringstream ss(line);
            std::vector <std::string> record;
            while(static_cast<bool>(ss))
            {
                std::string s;
                if (!getline( ss, s, ',' ))
                {
                    break;
                }
                lineData.push_back( s );
            }
            tempLocation.mName = lineData[0];
            tempLocation.mLatitude = std::stod(lineData[1])*0.0174533;
            tempLocation.mLongitude = std::stod(lineData[2])*0.0174533;
            
            locations.push_back(tempLocation);
        }
    }
    return locations;
}

Population CreatePopulation(std::mt19937& rand, int popSize, int numLocations)
{
    Population population;
    population.mMembers.resize(popSize);
    
    std::generate(population.mMembers.begin(), population.mMembers.end(), [&numLocations,&rand]()
                  {
                      std::vector<int> temp;
                      temp.resize(numLocations);
                      std::iota(temp.begin(), temp.end(), 0);
                      std::shuffle(temp.begin() + 1, temp.end(),rand);
                      return temp;
                  });
    return population;
}

double CalcHaversine(Location loc1, Location loc2)
{
    double dlon = loc2.mLongitude - loc1.mLongitude;
    double dlat = loc2.mLatitude - loc1.mLatitude;
    double a = pow(sin(dlat / 2.0), 2.0) + cos(loc1.mLatitude)*cos(loc2.mLatitude)*pow(sin(dlon / 2.0), 2.0);
    double c = 2.0*atan2(sqrt(a), sqrt(1.0 - a));
    return 3961.0 * c;
}

std::vector<std::pair<int, double>> GetFitness(Population& pop, std::vector<Location>&locVec)
{
    std::vector<std::pair<int, double>> fitnessVec;

    for(int i = 0; i < pop.mMembers.size(); i++)
    {
        std::vector<double> diff;
        std::adjacent_difference(pop.mMembers[i].begin(), pop.mMembers[i].end(), std::back_inserter(diff), [&locVec] (const int& a, const int& b)->double
                                 {
                                     return CalcHaversine(locVec[a], locVec[b]);
                                 });
            double fitness = std::accumulate(diff.begin(), diff.end(),0.0 );
            double add = CalcHaversine(locVec[pop.mMembers[i].back()], locVec[pop.mMembers[i].front()]);
            fitness += add;
            fitnessVec.emplace_back(std::pair<int, double>(i, fitness));
    }

    return fitnessVec;
}
std::vector<std::pair<int, double>> Sort(std::vector<std::pair<int, double>> fitVec)
{
    std::sort(fitVec.begin(), fitVec.end(), [](std::pair<int, double> x, std::pair<int, double> y)
              {
                  return x.second < y.second;
              });
    return fitVec;
}

std::vector<double> GetProb(int size, std::vector<std::pair<int, double>>& fitVec)
{
    std::vector<double> prob(size);
    std::generate(prob.begin(), prob.end(), [&size]()
        {
            return 1.0/size;
        });
    
    prob[fitVec[0].first] *= 6.0;
    prob[fitVec[1].first] *= 6.0;

    for (unsigned int i = 2; i < fitVec.size() / 2; i++)
    {
        prob[fitVec[i].first] *= 3.0;
    }
    double sum = std::accumulate(prob.begin(), prob.end(), 0.0, [](double x, double y)
                                 {
                                     return x + y;
                                 });
    std::transform(prob.begin(), prob.end(), prob.begin(),
                   [&sum](const double x)
                   {
                       return x / sum;
                   });
    return prob;
}

std::vector<std::pair<int, int>> GetPairs(std::vector<std::pair<int,double>>& sortedVec, std::vector<std::pair<int, double>>& fitVec, int popSize, std::mt19937& rand)
{
    std::vector<std::pair<int, int>> pairVec(popSize);
    std::vector<double> probVec = GetProb(popSize,sortedVec);
    
    std::generate(pairVec.begin(), pairVec.end(), [&probVec, &fitVec, &rand] ()
                  {
                      std::pair<int, int> pair;
                      std::uniform_real_distribution<double> urd(0.0, 1.0);
                      double randGen = urd(rand);
                      double sum = 0.0;
                      unsigned int i = 0;
                      while(i < probVec.size())
                      {
                          sum += probVec[i];
                          if(sum>=randGen)
                          {
                              break;
                          }
                          i++;
                      }

                      pair.first = fitVec[i].first;
                      
                      randGen = urd(rand);
                      sum = 0.0;
                      i = 0;
                      while(i < probVec.size())
                      {
                          sum += probVec[i];
                          if(sum>=randGen)
                          {
                              break;
                          }
                          i++;
                      }
                      pair.second= fitVec[i].first;
                      return pair;
                  }
                  );
    return pairVec;
}

Population CrossPopulation(std::vector<std::pair<int, int>>& selectedPairs, Population& pop, std::mt19937& rand, double mutationThresh)
{
    Population nPop;
    
    for (unsigned int i = 0; i < selectedPairs.size(); i++)
    {
        std::vector<int> next;
        std::vector<int> parentA = pop.mMembers[selectedPairs[i].first];
        std::vector<int> parentB = pop.mMembers[selectedPairs[i].second];
        std::uniform_int_distribution<int> randGen1(1, parentA.size() - 2);
        int crossIndex = randGen1(rand);
        std::uniform_int_distribution<int> coin(0, 1);
        int coinFlip = coin(rand);
        if (coinFlip != 0)
        {
            std::copy_n(parentA.begin(), crossIndex+1, std::back_inserter(next));
            std::copy_if(parentB.begin(), parentB.end(), std::back_inserter(next), [&next](const int a)
                         {
                             return std::find(next.begin(), next.end(), a) == next.end();
                         });
        }
        else
        {
            std::copy_n(parentB.begin(), crossIndex+1, std::back_inserter(next));
            std::copy_if(parentA.begin(), parentA.end(), std::back_inserter(next), [&next](const int a)
                         {
                             return std::find(next.begin(), next.end(), a) == next.end();
                         });
        }
        std::uniform_real_distribution<double> urd(0.0, 1.0);
        double mutChance = urd(rand);
        if (mutChance <= mutationThresh)
        {
            std::uniform_int_distribution<int> randGen2(1, next.size() - 1);
            int first = randGen2(rand);
            int second = randGen2(rand);
            std::swap(next[first], next[second]);
        }
        nPop.mMembers.push_back(next);
    }
    
    return nPop;
}

void WriteGeneration(std::ofstream& ofile, Population pop)
{
    for (int i = 0; i < pop.mMembers.size(); i++)
    {
        for (int j = 0; j < pop.mMembers[i].size(); j++)
        {
            if(j == pop.mMembers[i].size() - 1)
            {
                ofile << pop.mMembers[i][j];
            }
            else
            {
                ofile << pop.mMembers[i][j] << ",";
            }
        }
        ofile << std::endl;
    }
}

void WriteSelectedPairs(std::ofstream& ofile, std::vector<std::pair<int, int>> selectedVec)
{
    for(int i = 0; i < selectedVec.size(); i++)
    {
        ofile << "(" << selectedVec[i].first << "," << selectedVec[i].second << ")" << std::endl;
    }
}

void WriteSolution(std::ofstream& ofile, std::vector<Location> locationVec, Population pop, std::vector<std::pair<int, double>> sortedVec)
{
    for(int j = 0; j < pop.mMembers[sortedVec[0].first].size(); j++)
    {
        ofile << locationVec[pop.mMembers[sortedVec[0].first][j]].mName << std::endl;
    }
}
