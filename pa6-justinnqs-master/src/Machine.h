#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include "Op.h"
#include "Exceptions.h"
#ifdef _MSC_VER
#pragma warning(disable : 4996)
#endif
#include <wx/log.h>

// Defines state data accessible by the machine and ops
struct MachineState
{
	// Friend Machine so BindState works
	template <typename MachineTraits>
	friend class Machine;
	
	enum Facing { UP, RIGHT, DOWN, LEFT};
	MachineState()
		: mProgramCounter(1)
		, mActionsTaken(0)
		, mFacing(UP)
		, mTest(false)
	{
	}

	// Active line number in behavior program
	int mProgramCounter;
	// Number of actions taken this turn
	int mActionsTaken;
	// Current facing of this character
	Facing mFacing;
	// Test flag for branches
	bool mTest;
    // X Coordinate
    int mXCoord = 10;
    // Y Coordinate
    int mYCoord = 10;
    // GridX
    int mGridX;
    // GridY
    int mGridY;
    
	int GetActionsPerTurn() const throw() { return mActionsPerTurn; }
	bool GetInfect() const throw() { return mInfectOnAttack; }
private:
	// Data which is set by the traits
	int mActionsPerTurn = 0;
	bool mInfectOnAttack = false;
};

// Describes the machine which processes ops.
// Different policies dictate behavior possible for machine.
template <typename MachineTraits>
class Machine
{
public:
	// Clear out the current machine
	void ClearMachine();
	
	// Load in all the ops for this machine from the specified file
	void LoadMachine(const std::string& filename);

	// Given the state, binds the trait parameters to it
	void BindState(MachineState& state);

	// Take a turn using this logic for the passed in state
	void TakeTurn(MachineState& state);


	const std::string& GetName() const { return mName; }
private:
	std::vector<std::unique_ptr<Op>> mOps;
	std::string mName;
};

template <typename MachineTraits>
void Machine<MachineTraits>::ClearMachine()
{
	mOps.clear();
	mName.clear();
}

template <typename MachineTraits>
void Machine<MachineTraits>::LoadMachine(const std::string& filename)
{
//    // TEMP CODE: Add your parsing code here!
//    mOps.clear();
//    mOps.push_back(std::make_unique<OpRotate>(0));
//    mOps.push_back(std::make_unique<OpRotate>(0));
//    mOps.push_back(std::make_unique<OpRotate>(1));
//    mOps.push_back(std::make_unique<OpGoto>(1));
//    // END TEMP CODE
    mOps.clear();
    std::ifstream inFile (filename);
    std::string line;
    if(inFile.is_open())
    {
        // Parsing the operations from the file
        while(getline(inFile, line))
        {
            // Separates the comments from the operations and parameters
            std::string opString;
            std::string paramString;
            opString = line.substr(0, line.find(";", 0));
            paramString = opString.substr(line.find(",") + 1);
            opString = opString.substr(0, line.find(",", 0));
            if(opString == paramString)
            {
                paramString = "";
            }
            // Remove whitespace from strings
            // Removes all spaces from the beginning of the string
            while(opString.size() && isspace(opString.front()))
                opString.erase(opString.begin());
            // Remove all spaces from the end of the string.
            while(opString.size() && isspace(opString.back()))
                opString.pop_back();
            // Removes all spaces from the beginning of the string
            while(paramString.size() && isspace(paramString.front()))
                paramString.erase(paramString.begin());
            // Remove all spaces from the end of the string.
            while(paramString.size() && isspace(paramString.back()))
                paramString.pop_back();
            
            // Adds the appropriate ops to the machine
            if(opString == "test_wall")
            {
                mOps.push_back(std::make_unique<OpTestWall>(0));
            }
            else if(opString == "je")
            {
                mOps.push_back(std::make_unique<OpJe>(std::stoi(paramString)));
            }
            else if(opString == "forward")
            {
                mOps.push_back(std::make_unique<OpForward>(0));
            }
            else if(opString == "goto")
            {
                mOps.push_back(std::make_unique<OpGoto>(std::stoi(paramString)));
            }
            else if(opString == "test_random")
            {
                mOps.push_back(std::make_unique<OpTestRandom>(0));
            }
            else if(opString == "rotate")
            {
                mOps.push_back(std::make_unique<OpRotate>(std::stoi(paramString)));
            }
        }
        inFile.close();
    }
}

template <typename MachineTraits>
void Machine<MachineTraits>::BindState(MachineState& state)
{
	state.mActionsPerTurn = MachineTraits::ACTIONS_PER_TURN;
	state.mInfectOnAttack = MachineTraits::INFECT_ON_ATTACK;
}

template <typename MachineTraits>
void Machine<MachineTraits>::TakeTurn(MachineState& state)
{
	wxLogDebug("TAKING TURN");
	state.mActionsTaken = 0;
	while (state.mActionsTaken < MachineTraits::ACTIONS_PER_TURN)
	{
		mOps.at(state.mProgramCounter - 1)->Execute(state);
        std::cout << state.mXCoord << " " << state.mYCoord << std::endl;
	}
}
