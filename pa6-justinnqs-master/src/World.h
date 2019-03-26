#pragma once
#include <string>
#include <vector>
#include "Machine.h"
#include "Traits.h"
#include "Singleton.h"

class World : public Singleton<World>
{
public:
    Machine<HumanTraits> mSurvivorMachine;
    Machine<ZombieTraits> mZombieMachine;
    std::vector<MachineState> mZombieStates;
    std::vector<MachineState> mSurvivorStates;
    MachineState mGrid[20][20];
    int mOccupiedGrid[20][20];
};
