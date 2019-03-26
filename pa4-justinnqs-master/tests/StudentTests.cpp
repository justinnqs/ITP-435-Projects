#include "catch.hpp"
#include "SrcMain.h"
#include "Solver.h"
#include <string>
#include <chrono>

// Helper function declarations (don't change these)
extern bool CheckFileMD5(const std::string& fileName, const std::string& expected);
extern bool CheckTextFilesSame(const std::string& fileNameA, 
	const std::string& fileNameB);

// TODO:
// Add test cases for your functions here!!
// (You will want to make multiple test cases with different sections)
TEST_CASE("Function test cases", "[student]")
{
    SECTION("Look up test")
    {
        const char* argv[] = {
            "tests/tests",
            "input/d2.txt",
            "input/pass-dict.txt"
        };
        Solver solver;
        solver.ParseDictionary(argv[1]);
        solver.ParsePassword(argv[2]);
        solver.LookUp();
        REQUIRE(solver.GetmPassDecr()[0] == "123456");
    }
    
    SECTION("Brute Force test")
    {
        const char* argv[] = {
            "tests/tests",
            "input/d2.txt",
            "input/pass-brute.txt"
        };
        Solver solver;
        solver.ParseDictionary(argv[1]);
        solver.ParsePassword(argv[2]);
        solver.LookUp();
        solver.ParBruteForce();
        REQUIRE(solver.GetmPassDecr()[0] == "sj12");
    }
    
    SECTION("Decode Counter")
    {
        Solver solver;
        REQUIRE(solver.DecodeCounter({0,0,0,0}) == "aaaa");
    }
    
    SECTION("Get Hash")
    {
        Solver solver;
        REQUIRE(solver.GetHash("sj12") == "a78cb8df129f590ee667a7a0949327760917344c");
    }
}

// Provided file tests
// (Once you pass the first section, comment out the other)
TEST_CASE("File tests", "[student]")
{
    SECTION("Dictionary only")
    {
        const char* argv[] = {
            "tests/tests",
            "input/d2.txt",
            "input/pass-dict.txt"
        };
        ProcessCommandArgs(3, argv);
        bool result = CheckTextFilesSame("solved.txt", "expected/dict-solved.txt");
        REQUIRE(result);
    }

    SECTION("Brute force only")
    {
        const char* argv[] = {
            "tests/tests",
            "input/d2.txt",
            "input/pass-brute.txt"
        };
        ProcessCommandArgs(3, argv);
        bool result = CheckTextFilesSame("solved.txt", "expected/brute-solved.txt");
        REQUIRE(result);
    }

    SECTION("Full")
    {
        const char* argv[] = {
            "tests/tests",
            "input/d8.txt",
            "input/pass-full.txt"
        };
        ProcessCommandArgs(3, argv);
        bool result = CheckTextFilesSame("solved.txt", "expected/full-solved.txt");
        REQUIRE(result);
    }

    SECTION("Full - Timed in Release")
    {
        const char* argv[] = {
            "tests/tests",
            "input/d8.txt",
            "input/pass-full.txt"
        };
        auto start = std::chrono::high_resolution_clock::now();
        ProcessCommandArgs(3, argv);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        float seconds = duration / 1000000000.0f;
        bool result = CheckTextFilesSame("solved.txt", "expected/full-solved.txt");
        REQUIRE(result);
        WARN("****Full timed test took: " << seconds << "s****");
        REQUIRE(seconds < 2.5f);
    }
}
