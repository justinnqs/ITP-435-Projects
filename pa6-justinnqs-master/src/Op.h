// Defines all the classes for different operations
// which can be performed.
#pragma once

struct MachineState;

// Abstract Base Class
class Op
{
public:
	Op(const wchar_t* OpName, int parameter)
		: mParam(parameter)
		, mOpName(OpName)
	{ }

	void DebugOutput(MachineState& state);

	virtual void Execute(MachineState& state) = 0;
protected:
	int mParam;
	const wchar_t* mOpName;
};

// Defines the rotate operation
// Param of 0 means rotate clockwise
// Param of 1 means rotate counter-clockwise
struct OpRotate : Op
{
	OpRotate(int parameter)
		: Op(L"OpRotate", parameter)
	{ }

	void Execute(MachineState& state) override;
};

// Defines the goto operation
// Parameter determines the line number
struct OpGoto : Op
{
	OpGoto(int parameter)
		: Op(L"OpGoto", parameter)
	{ }

	void Execute(MachineState& state) override;
};

// Defines the test_wall operation
// Parameter is a dummy
struct OpTestWall : Op
{
    OpTestWall(int parameter)
    : Op(L"OpTestWall", parameter)
    { }
    
    void Execute(MachineState& state) override;
};

// Defines the test_random operation
// Parameter is a dummy
struct OpTestRandom : Op
{
    OpTestRandom(int parameter)
    : Op(L"OpTestRandom", parameter)
    { }
    
    void Execute(MachineState& state) override;
};

// Defines the je operation
// Parameter determines the line number
struct OpJe : Op
{
    OpJe(int parameter)
        : Op(L"OpJe", parameter)
    { }
    
    void Execute(MachineState& state) override;
};

// Defines the forward operation
// Parameter is a dummy
struct OpForward : Op
{
    OpForward(int parameter)
    : Op(L"OpForward", parameter)
    { }
    
    void Execute(MachineState& state) override;
};

struct OpAttack : Op
{
    OpAttack(int parameter)
    : Op(L"OpAttack", parameter)
    { }

    void Execute(MachineState& state) override;
};

//struct OpRangedAttack : Op
//{
//    OpRangedAttack(int parameter)
//    : Op(L"OpRangedAttack", parameter)
//    { }
//    
//    void Execute(MachineState& state) override;
//};

struct OpEndTurn : Op
{
    OpEndTurn(int parameter)
    : Op(L"OpEndTurn", parameter)
    { }
    
    void Execute(MachineState& state) override;
};

struct OpTestHuman : Op
{
    OpTestHuman(int parameter)
    : Op(L"OpTestHuman", parameter)
    { }
    
    void Execute(MachineState& state) override;
};

struct OpTestZombie : Op
{
    OpTestZombie(int parameter)
    : Op(L"OpTestZombie", parameter)
    { }
    
    void Execute(MachineState& state) override;
};

struct OpTestPassable : Op
{
    OpTestPassable(int parameter)
    : Op(L"OpTestPassable", parameter)
    { }
    
    void Execute(MachineState& state) override;
};

struct OpJne : Op
{
    OpJne(int parameter)
    : Op(L"OpJne", parameter)
    { }
    
    void Execute(MachineState& state) override;
};

