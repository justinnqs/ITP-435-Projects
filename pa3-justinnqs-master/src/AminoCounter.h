#pragma once
#include <unordered_map>
#include <string>
#include <sstream>
#include <iostream>

class Amino
{
public:
    Amino()
    {
        
    }
    void Counter(std::string sequence);
    std::unordered_map<int, int> GetAminoCount();
    int GetSpecAmino(int aminoID);
    int GetTotal();
private:
    /*
     Legend
     (A) Alanine: 24
     (C) Cysteine: 25
     (D) Aspartic acid: 26
     (E) Glutamic acid: 27
     (F) Phenylalanine: 28
     (G) Glycine: 29
     (H) Histidine: 30
     (I) Isoleucine: 31
     (K) Lysine: 32
     (L) Leucine: 33
     (M) Methionine: 34
     (N) Asparagine: 35
     (P) Proline: 36
     (Q) Glutamine: 37
     (R) Arginine: 38
     (S) Serine: 39
     (T) Threonine: 40
     (V) Valine: 41
     (W) Tryptophan: 42
     (Y) Tyrosine: 43
    */
    int mTotalAmino;
    std::unordered_map<int, int> mAminoCount = {
            {24, 0},
            {25, 0},
            {26, 0},
            {27, 0},
            {28, 0},
            {29, 0},
            {30, 0},
            {31, 0},
            {32, 0},
            {33, 0},
            {34, 0},
            {35, 0},
            {36, 0},
            {37, 0},
            {38, 0},
            {39, 0},
            {40, 0},
            {41, 0},
            {42, 0},
            {43, 0}};
    int mStateMach[24][4] = {
            {0, 0, 1, 0},
            {2, 0, 1, 0},
            {0, 0, 1, 34},
            {4, 9, 14, 19},
            {5, 6, 7, 8},
            {28, 28, 33, 33},
            {39, 39, 39, 39},
            {43, 43, 0, 0},
            {25, 25, 0, 42},
            {10, 11, 12, 13},
            {33, 33, 33, 33},
            {36, 36, 36, 36},
            {30, 30, 37, 37},
            {38, 38, 38, 38},
            {15, 16, 17, 18},
            {31, 31, 31, 34},
            {40, 40, 40, 40},
            {35, 35, 32, 32},
            {39, 39, 38, 38},
            {20, 21, 22, 23},
            {41, 41, 41, 41},
            {24, 24, 24, 24},
            {26, 26, 27, 27},
            {29, 29, 29, 29}};
    
};
