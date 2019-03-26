#include "Op.h"
#include "Machine.h"
#include "World.h"
#ifdef _MSC_VER
#pragma warning(disable : 4996)
#endif
#include <wx/log.h>

// Output state information for debugging purposes
void Op::DebugOutput(MachineState& state)
{
	wxLogDebug("%d:%s,%d", state.mProgramCounter, mOpName, mParam);
}

void OpRotate::Execute(MachineState& state)
{
	DebugOutput(state);
	switch (state.mFacing)
	{
	case (MachineState::UP):
		if (mParam == 0)
		{
			state.mFacing = MachineState::RIGHT;
		}
		else
		{
			state.mFacing = MachineState::LEFT;
		}
		break;
	case (MachineState::RIGHT):
		if (mParam == 0)
		{
			state.mFacing = MachineState::DOWN;
		}
		else
		{
			state.mFacing = MachineState::UP;
		}
		break;
	case (MachineState::DOWN):
		if (mParam == 0)
		{
			state.mFacing = MachineState::LEFT;
		}
		else
		{
			state.mFacing = MachineState::RIGHT;
		}
		break;
	case (MachineState::LEFT):
		if (mParam == 0)
		{
			state.mFacing = MachineState::UP;
		}
		else
		{
			state.mFacing = MachineState::DOWN;
		}
		break;
	}

	state.mProgramCounter++;
	state.mActionsTaken++;
}

void OpGoto::Execute(MachineState& state)
{
	DebugOutput(state);
	state.mProgramCounter = mParam;
}

void OpJe::Execute(MachineState& state)
{
    DebugOutput(state);
    if(state.mTest == true)
    {
        state.mProgramCounter = mParam;
    }
    else
    {
        state.mProgramCounter++;
    }
}

void OpTestWall::Execute(MachineState& state)
{
    DebugOutput(state);
    if(state.mYCoord == 10 && state.mFacing == MachineState::UP)
    {
        state.mTest = true;
    }
    else if(state.mXCoord == 10 && state.mFacing == MachineState::LEFT)
    {
        state.mTest = true;
    }
    else if(state.mYCoord == 580 && state.mFacing == MachineState::DOWN)
    {
        state.mTest = true;
    }
    else if(state.mXCoord == 580 && state.mFacing == MachineState::RIGHT)
    {
        state.mTest = true;
    }
    else
    {
        state.mTest = false;
    }
    state.mProgramCounter++;
}

void OpTestRandom::Execute(MachineState& state)
{
    DebugOutput(state);
    int flip;
    flip = rand() % 2 + 1;// assign random numbers
    if(flip == 1)
    {
        state.mTest = true;
    }
    else
    {
        state.mTest = false;
    }
    state.mProgramCounter++;
}

void OpForward::Execute(MachineState& state)
{
    DebugOutput(state);
    switch (state.mFacing)
    {
        case (MachineState::UP) :
            if(World::Get().mOccupiedGrid[state.mGridX][state.mGridY-1] == 0)
            {
                state.mYCoord -= 30;
                World::Get().mOccupiedGrid[state.mGridX][state.mGridY-1] = World::Get().mOccupiedGrid[state.mGridX][state.mGridY];
                World::Get().mOccupiedGrid[state.mGridX][state.mGridY] = 0;
                World::Get().mGrid[state.mGridX][state.mGridY-1] = state;
                MachineState clear;
                World::Get().mGrid[state.mGridX][state.mGridY] = clear;
                state.mGridY--;
            }
            break;
        case (MachineState::DOWN) :
            if(World::Get().mOccupiedGrid[state.mGridX][state.mGridY+1] == 0)
            {
                state.mYCoord += 30;
                World::Get().mOccupiedGrid[state.mGridX][state.mGridY+1] = World::Get().mOccupiedGrid[state.mGridX][state.mGridY];
                World::Get().mOccupiedGrid[state.mGridX][state.mGridY] = 0;
                World::Get().mGrid[state.mGridX][state.mGridY+1] = state;
                MachineState clear;
                World::Get().mGrid[state.mGridX][state.mGridY] = clear;
                state.mGridY++;
            }
            break;
        case (MachineState::LEFT) :
            if(World::Get().mOccupiedGrid[state.mGridX-1][state.mGridY] == 0)
            {
                state.mXCoord -= 30;
                World::Get().mOccupiedGrid[state.mGridX-1][state.mGridY] = World::Get().mOccupiedGrid[state.mGridX][state.mGridY];
                World::Get().mOccupiedGrid[state.mGridX][state.mGridY] = 0;
                World::Get().mGrid[state.mGridX-1][state.mGridY] = state;
                MachineState clear;
                World::Get().mGrid[state.mGridX][state.mGridY] = clear;
                state.mGridX--;
            }
            break;
        case (MachineState::RIGHT) :
            if(World::Get().mOccupiedGrid[state.mGridX+1][state.mGridY] == 0)
            {
                state.mXCoord += 30;
                World::Get().mOccupiedGrid[state.mGridX+1][state.mGridY] = World::Get().mOccupiedGrid[state.mGridX][state.mGridY];
                World::Get().mOccupiedGrid[state.mGridX][state.mGridY] = 0;
                World::Get().mGrid[state.mGridX+1][state.mGridY] = state;
                MachineState clear;
                World::Get().mGrid[state.mGridX][state.mGridY] = clear;
                state.mGridX++;
            }
            break;
    }
    state.mProgramCounter++;
    state.mActionsTaken++;
}

void OpAttack::Execute(MachineState &state)
{
    
}

//void OpRangedAttack::Execute(MachineState &state)
//{
//    
//}


void OpEndTurn::Execute(MachineState &state)
{
    state.mActionsTaken = state.GetActionsPerTurn();
}

void OpTestHuman::Execute(MachineState &state)
{
    if(mParam == 1)
    {
        switch (state.mFacing)
        {
            case (MachineState::UP) :
                if(World::Get().mOccupiedGrid[state.mGridX][state.mGridY-1] == 2)
                {
                    state.mTest = true;
                }
                break;
            case (MachineState::DOWN) :
                if(World::Get().mOccupiedGrid[state.mGridX][state.mGridY+1] == 2)
                {
                    state.mTest = true;
                }
                break;
            case (MachineState::RIGHT) :
                if(World::Get().mOccupiedGrid[state.mGridX+1][state.mGridY] == 2)
                {
                    state.mTest = true;
                }
                break;
            case (MachineState::LEFT) :
                if(World::Get().mOccupiedGrid[state.mGridX-1][state.mGridY] == 2)
                {
                    state.mTest = true;
                }
                break;
        }
    }
    if(mParam == 2)
    {
        switch (state.mFacing)
        {
            case (MachineState::UP) :
                if(World::Get().mOccupiedGrid[state.mGridX][state.mGridY-2] == 2)
                {
                    state.mTest = true;
                }
                break;
            case (MachineState::DOWN) :
                if(World::Get().mOccupiedGrid[state.mGridX][state.mGridY+2] == 2)
                {
                    state.mTest = true;
                }
                break;
            case (MachineState::RIGHT) :
                if(World::Get().mOccupiedGrid[state.mGridX+2][state.mGridY] == 2)
                {
                    state.mTest = true;
                }
                break;
            case (MachineState::LEFT) :
                if(World::Get().mOccupiedGrid[state.mGridX-2][state.mGridY] == 2)
                {
                    state.mTest = true;
                }
                break;
        }
    }
}

void OpTestZombie::Execute(MachineState &state)
{
    if(mParam == 1)
    {
        switch (state.mFacing)
        {
            case (MachineState::UP) :
                if(World::Get().mOccupiedGrid[state.mGridX][state.mGridY-1] == 1)
                {
                    state.mTest = true;
                }
                break;
            case (MachineState::DOWN) :
                if(World::Get().mOccupiedGrid[state.mGridX][state.mGridY+1] == 1)
                {
                    state.mTest = true;
                }
                break;
            case (MachineState::RIGHT) :
                if(World::Get().mOccupiedGrid[state.mGridX+1][state.mGridY] == 1)
                {
                    state.mTest = true;
                }
                break;
            case (MachineState::LEFT) :
                if(World::Get().mOccupiedGrid[state.mGridX-1][state.mGridY] == 1)
                {
                    state.mTest = true;
                }
                break;
        }
    }
    if(mParam == 2)
    {
        switch (state.mFacing)
        {
            case (MachineState::UP) :
                if(World::Get().mOccupiedGrid[state.mGridX][state.mGridY-2] == 1)
                {
                    state.mTest = true;
                }
                break;
            case (MachineState::DOWN) :
                if(World::Get().mOccupiedGrid[state.mGridX][state.mGridY+2] == 1)
                {
                    state.mTest = true;
                }
                break;
            case (MachineState::RIGHT) :
                if(World::Get().mOccupiedGrid[state.mGridX+2][state.mGridY] == 1)
                {
                    state.mTest = true;
                }
                break;
            case (MachineState::LEFT) :
                if(World::Get().mOccupiedGrid[state.mGridX-2][state.mGridY] == 1)
                {
                    state.mTest = true;
                }
                break;
        }
    }
}

void OpTestPassable::Execute(MachineState &state)
{
    switch (state.mFacing)
    {
        case (MachineState::UP) :
            if(World::Get().mOccupiedGrid[state.mGridX][state.mGridY-1] == 0)
            {
                state.mTest = true;
            }
            break;
        case (MachineState::DOWN) :
            if(World::Get().mOccupiedGrid[state.mGridX][state.mGridY+1] == 0)
            {
                state.mTest = true;
            }
            break;
        case (MachineState::RIGHT) :
            if(World::Get().mOccupiedGrid[state.mGridX+1][state.mGridY] == 0)
            {
                state.mTest = true;
            }
            break;
        case (MachineState::LEFT) :
            if(World::Get().mOccupiedGrid[state.mGridX-1][state.mGridY] == 0)
            {
                state.mTest = true;
            }
            break;
    }
}

void OpJne::Execute(MachineState &state)
{
    DebugOutput(state);
    if(state.mTest == false)
    {
        state.mProgramCounter = mParam;
    }
    else
    {
        state.mProgramCounter++;
    }
}

