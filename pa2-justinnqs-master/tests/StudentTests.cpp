#include "catch.hpp"
#include "SrcMain.h"
#include "TSP.h"
#include <string>
#include <iostream>
#include <fstream>

// Don't change this
#ifdef _MSC_VER
const std::string SamplePath("sampleoutput/win/");
#elif __APPLE__
const std::string SamplePath("sampleoutput/mac/");
#else
const std::string SamplePath("sampleoutput/linux/");
#endif
// Helper function declarations (don't change these)
extern bool CheckFileMD5(const std::string& fileName, const std::string& expected);
extern bool CheckTextFilesSame(const std::string& fileNameA, 
	const std::string& fileNameB);

// TODO:
// Add test cases for your functions here!!
// (You will want to make multiple test cases with different sections)
TEST_CASE("Function test cases", "[student]")
{
    SECTION("CalcHaversine function")
    {
        bool result = false;
        Location loc1, loc2;
        loc1.mLatitude = 0.123;
        loc1.mLongitude = 0.123;
        
        loc2.mLatitude = 0.234;
        loc2.mLongitude = 0.342;
        REQUIRE(static_cast<int>(CalcHaversine(loc1, loc2)) == 959);
    }
    
    SECTION("GetFitness function")
    {
        Location loc1, loc2;
        loc1.mLatitude = 0.123;
        loc1.mLongitude = 0.123;
        
        loc2.mLatitude = 0.234;
        loc2.mLongitude = 0.342;
        std::vector<Location> locVec;
        locVec.push_back(loc1);
        locVec.push_back(loc2);
        Population pop;
        std::vector<std::vector<int>> temp;
        std::vector<int> val;
        val.push_back(0);
        val.push_back(1);
        temp.push_back(val);
        pop.mMembers = temp;

        REQUIRE(static_cast<int>(GetFitness(pop, locVec)[0].second) == 1919);
    }
    

    SECTION("CrossPopulation function")
    {
        std::string inputFile = "input/locations.txt";
        std::vector<Location> locationVec = ParseLocations(inputFile);
        std::mt19937 rand(1337);
        std::ofstream ofile("cross.txt");
        Population pop = CreatePopulation(rand, 8, locationVec.size());
        
        WriteGeneration(ofile, pop);
        bool result = CheckTextFilesSame("cross.txt", "check.txt");
        REQUIRE(result);
    }

    SECTION("Sort Function")
    {
        std::vector<std::pair<int, double>> temp;
        temp.push_back(std::make_pair(0, 0.1));
        temp.push_back(std::make_pair(1, 0.2));
                       
        REQUIRE(Sort(temp)[0].first == 0);
    }

}

// Provided Complete file test cases
// (Once you pass the first section, comment out the other)
TEST_CASE("Full file test cases", "[student]")
{
	SECTION("input/locations.txt 8 5 10 1337")
	{
		const char* argv[] = {
			"tests/tests",
			"input/locations.txt",
			"8",
			"5",
			"10",
			"1337"
		};
		ProcessCommandArgs(6, argv);
		bool result = CheckTextFilesSame("log.txt", SamplePath + "log_8_5_10_1337.txt");
		REQUIRE(result);
	}
    SECTION("input/locations.txt 32 20 5 5741328")
    {
        const char* argv[] = {
            "tests/tests",
            "input/locations.txt",
            "32",
            "20",
            "5",
            "5741328"
        };
        ProcessCommandArgs(6, argv);
        bool result = CheckTextFilesSame("log.txt", SamplePath + "log_32_20_5_5741328.txt");
        REQUIRE(result);
    }
    SECTION("input/locations.txt 64 500 30 12165465")
    {
        const char* argv[] = {
            "tests/tests",
            "input/locations.txt",
            "64",
            "500",
            "30",
            "12165465"
        };
        ProcessCommandArgs(6, argv);
        bool result = CheckTextFilesSame("log.txt", SamplePath + "log_64_500_30_12165465.txt");
        REQUIRE(result);
    }
    SECTION("input/locations.txt 128 200 10 331988960")
    {
        const char* argv[] = {
            "tests/tests",
            "input/locations.txt",
            "128",
            "200",
            "10",
            "331988960"
        };
        ProcessCommandArgs(6, argv);
        bool result = CheckTextFilesSame("log.txt", SamplePath + "log_128_200_10_331988960.txt");
        REQUIRE(result);
    }
    SECTION("input/locations.txt 256 200 20 7410785")
    {
        const char* argv[] = {
            "tests/tests",
            "input/locations.txt",
            "256",
            "200",
            "20",
            "7410785"
        };
        ProcessCommandArgs(6, argv);
        bool result = CheckTextFilesSame("log.txt", SamplePath + "log_256_200_20_7410785.txt");
        REQUIRE(result);
    }
    SECTION("input/locations2.txt 64 200 25 49328573")
    {
        const char* argv[] = {
            "tests/tests",
            "input/locations2.txt",
            "64",
            "200",
            "25",
            "49328573"
        };
        ProcessCommandArgs(6, argv);
        bool result = CheckTextFilesSame("log.txt", SamplePath + "log_64_200_25_49328573.txt");
        REQUIRE(result);
    }
}
