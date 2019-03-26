#include "catch.hpp"
#include "RleData.h"
#include "RleFile.h"
#include "SrcMain.h"

// Helper function declarations (don't change these)
extern bool BuffersAreSame(const char* expected, const char* actual, size_t size);
extern bool RunCompressionTest(char* test, size_t testSize,
							   char* expected, size_t expectedSize);
extern bool RunDecompressionTest(char* test, size_t testSize,
								 char* expected, size_t expectedSize);
extern bool CheckFileMD5(const std::string& fileName, const std::string& expected);

TEST_CASE("RLE Compression", "[student]")
{
	SECTION("Basic positive run")
	{
		char test[] = "aaabbbcccdddaaabbbcccdddaaabbbcccdddaaabbbc"
		"ccdddaaabbbcccdddaaabbbcccdddaaabbbcccdddaaabbbcccddd";
		char expected[] = "\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
		"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
		"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
		"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
		"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
		"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
		"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
		"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d";
		
		bool result = RunCompressionTest(test, sizeof(test) - 1,
										 expected, sizeof(expected) - 1);
		REQUIRE(result);
	}
	
	// TODO: Add more test case sections!!
    SECTION("Basic negative run")
    {
        char test[] = "abcdefghijklmnopqrstuvwxyz";
        char expected[] = "\xe6" "abcdefghijklmnopqrstuvwxyz";

        bool result = RunCompressionTest(test, sizeof(test) - 1,
                                         expected, sizeof(expected) - 1);
        REQUIRE(result);
    }
    
    SECTION("Negative and Positive run one after the other")
    {
        char test[] = "abcdefghijklmnopqrstuvwxyz" "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
        char expected[] = "\xe6" "abcdefghijklmnopqrstuvwxyz" "\x28" "x";
        
        bool result = RunCompressionTest(test, sizeof(test) - 1,
                                         expected, sizeof(expected) - 1);
        REQUIRE(result);
    }
    
    SECTION("Negative and Positive run mixed")
    {
        char test[] = "aaa" "ghi" "bbb" "ghi" "ccc" "ghi";
        char expected[] = "\x3" "a" "\xfd" "ghi" "\x3" "b" "\xfd" "ghi" "\x3" "c" "\xfd" "ghi";
        
        bool result = RunCompressionTest(test, sizeof(test) - 1,
                                         expected, sizeof(expected) - 1);
        REQUIRE(result);
    }
    
    SECTION("Run of 1")
    {
        char test[] = "aaabaaa";
        char expected[] = "\x3" "a" "\x1" "b" "\x3" "a";
        
        bool result = RunCompressionTest(test, sizeof(test) - 1,
                                         expected, sizeof(expected) - 1);
        REQUIRE(result);
    }
    
    SECTION("Run of all 0")
    {
        char test[] = "0000000000";
        char expected[] = "\xa" "0";
        
        bool result = RunCompressionTest(test, sizeof(test) - 1,
                                         expected, sizeof(expected) - 1);
        REQUIRE(result);
    }
    
    SECTION("Long positive run with 1 byte left over")
    {
        char test[] = "aaaaaaaaaa" "aaaaaaaaaa" "aaaaaaaaaa" "aaaaaaaaaa" "aaaaaaaaaa"
        "aaaaaaaaaa" "aaaaaaaaaa" "aaaaaaaaaa" "aaaaaaaaaa" "aaaaaaaaaa"
        "aaaaaaaaaa" "aaaaaaaaaa" "aaaaaaaa";
        char expected[] = "\x7f" "a" "\x1" "a";
        
        bool result = RunCompressionTest(test, sizeof(test) - 1,
                                         expected, sizeof(expected) - 1);
        REQUIRE(result);
    }
    
    SECTION("Long positive run with short negative run")
    {
        char test[] = "aaaaaaaaaa" "aaaaaaaaaa" "aaaaaaaaaa" "aaaaaaaaaa" "aaaaaaaaaa"
        "aaaaaaaaaa" "aaaaaaaaaa" "aaaaaaaaaa" "aaaaaaaaaa" "aaaaaaaaaa"
        "aaaaaaaaaa" "aaaaaaaaaa" "aaaaaaa" "ghi";
        char expected[] = "\x7f" "a" "\xfd" "ghi";
        
        bool result = RunCompressionTest(test, sizeof(test) - 1,
                                         expected, sizeof(expected) - 1);
        REQUIRE(result);
    }
    
    SECTION("Long negative run with short positive run")
    {
        char test[] = "abcdefghij" "abcdefghij" "abcdefghij" "abcdefghij" "abcdefghij"
        "abcdefghij" "abcdefghij" "abcdefghij" "abcdefghij" "abcdefghij"
        "abcdefghij" "abcdefghij" "abcdefg" "aaa";
        char expected[] = "\x81" "abcdefghij" "abcdefghij" "abcdefghij" "abcdefghij" "abcdefghij"
        "abcdefghij" "abcdefghij" "abcdefghij" "abcdefghij" "abcdefghij"
        "abcdefghij" "abcdefghij" "abcdefg" "\x3" "a";
        
        bool result = RunCompressionTest(test, sizeof(test) - 1,
                                         expected, sizeof(expected) - 1);
        REQUIRE(result);
    }
}

TEST_CASE("RLE Decompression", "[student]")
{
	SECTION("Basic positive run")
	{
		char test[] = "\x28" "x";
		char expected[] = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
		
		bool result = RunDecompressionTest(test, sizeof(test) - 1,
										   expected, sizeof(expected) - 1);
		REQUIRE(result);
	}
	// TODO: Add more test case sections!!
    SECTION("Basic negative run")
    {
        char test[] = "\xe6" "abcdefghijklmnopqrstuvwxyz";
        char expected[] = "abcdefghijklmnopqrstuvwxyz";

        bool result = RunDecompressionTest(test, sizeof(test) - 1,
                                           expected, sizeof(expected) - 1);
        REQUIRE(result);
    }

    SECTION("Positive and Negative run 1")
    {
        char test[] = "\xe6" "abcdefghijklmnopqrstuvwxyz" "\x28" "x";
        char expected[] = "abcdefghijklmnopqrstuvwxyz"
        "xxxxxxxxxxxxxxxxxxxx"
        "xxxxxxxxxxxxxxxxxxxx";

        bool result = RunDecompressionTest(test, sizeof(test) - 1,
                                           expected, sizeof(expected) - 1);
        REQUIRE(result);
    }
    
    SECTION("Positive and Negative run 2")
    {
        char test[] = "\x03" "\0";
        char expected[] = "\0\0\0";
        
        bool result = RunDecompressionTest(test, sizeof(test) - 1,
                                           expected, sizeof(expected) - 1);
        REQUIRE(result);
    }
    
    SECTION("Positive null run")
    {
        char test[] = "\xf9" "abcdefg""\x28"  "x" "\xf9" "abcdefg";
        char expected[] = "abcdefg""xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx""abcdefg";
        
        bool result = RunDecompressionTest(test, sizeof(test) - 1,
                                           expected, sizeof(expected) - 1);
        REQUIRE(result);
    }
}

TEST_CASE("Command line arguments", "[student]")
{
	SECTION("instructions.txt.rl1")
	{
		const char* argv[] = {
			"tests/tests",
			"data/instructions.txt.rl1"
		};
		ProcessCommandArgs(2, argv);
		bool result = CheckFileMD5("data/instructions.txt", "5e042d2e4bf014e714dd7cfc4dc25aab");
		REQUIRE(result);
	}
}

TEST_CASE("File compression", "[student]")
{
	RleFile r;
	SECTION("rle.bmp")
	{
		r.CreateArchive("data/rle.bmp");
		bool result = CheckFileMD5("data/rle.bmp.rl1", "f2a9d8425d53c664e45d9eb1b53137b9");
		REQUIRE(result);
	}
	SECTION("pic.jpg")
	{
		r.CreateArchive("data/pic.jpg");
		bool result = CheckFileMD5("data/pic.jpg.rl1", "0bbf2a5109b30d79939d2061ea8c74aa");
		REQUIRE(result);
	}
	SECTION("Conquest.ogg")
	{
		r.CreateArchive("data/Conquest.ogg");
		bool result = CheckFileMD5("data/Conquest.ogg.rl1", "ec29ff368ec5100bfba22635ddc5ba5c");
		REQUIRE(result);
	}
}

TEST_CASE("File decompression", "[student]")
{
	RleFile r;
	SECTION("works.bmp.rl1")
	{
		r.ExtractArchive("data/works.bmp.rl1");
		bool result = CheckFileMD5("data/works.bmp", "8baad647acefae2f8c36ee111582a875");
		REQUIRE(result);
	}
	SECTION("xkcd.bmp.rl1")
	{
		r.ExtractArchive("data/xkcd.bmp.rl1");
		bool result = CheckFileMD5("data/xkcd.bmp", "a4d7efa89d47a94a76f8050dd8d60cd0");
		REQUIRE(result);
	}
	SECTION("logo.png.rl1")
	{
		r.ExtractArchive("data/logo.png.rl1");
		bool result = CheckFileMD5("data/logo.png", "95403309460632f60efa647ef59b78fc");
		REQUIRE(result);
	}
}

