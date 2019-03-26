#pragma once

#include <vector>
#include <string>
#include <map>
#include <iterator>
#include <iostream>

struct CodeContext
{
	std::vector<std::string> mOps;
    std::map<int, int> mGoTos;
};

class Node
{
public:
	virtual void CodeGen(CodeContext& context) const = 0;
};

class NNumeric : public Node
{
public:
	NNumeric(std::string& value);
	int mValue;
	void CodeGen(CodeContext& context) const override { }
};

class NStatement : public Node
{
};

class NRotate : public NStatement
{
public:
	NRotate(NNumeric* dir);
	void CodeGen(CodeContext& context) const override;
private:
	NNumeric* mDir;
};

class NBoolean : public Node
{
};

class NBlock : public Node
{
public:
	NBlock() { mMainBlock = false; }
	void CodeGen(CodeContext& context) const override;
	void AddStatement(NStatement* statement);
	void SetMainBlock() { mMainBlock = true; }
	void Optimize() { mOptimize = true; }
private:
	std::vector<NStatement*> mStatements;
	bool mMainBlock;
	bool mOptimize = false;
};

class NForward : public NStatement
{
public:
    void CodeGen(CodeContext& context) const override;
};

class NAttack : public NStatement
{
public:
    void CodeGen(CodeContext& context) const override;
};

class NRangedAttack : public NStatement
{
public:
    void CodeGen(CodeContext& context) const override;
};

class NIfElse : public NStatement
{
public:
    NIfElse(NBoolean* boolVal, NBlock* ifBlock, NBlock* elseBlock);
    void CodeGen(CodeContext& context) const override;
private:
    NBoolean* mBoolVal;
    NBlock* mIfBlock;
    NBlock* mElseBlock;
};

class IsHuman : public NBoolean
{
public:
    IsHuman(NNumeric* dir);
    void CodeGen(CodeContext& context) const override;
private:
    NNumeric* mDir;
};

class IsZombie : public NBoolean
{
public:
    IsZombie(NNumeric* dir);
    void CodeGen(CodeContext& context) const override;
private:
    NNumeric* mDir;
};

class IsPassable : public NBoolean
{
public:
    void CodeGen(CodeContext& context) const override;
};

class IsRandom : public NBoolean
{
public:
    void CodeGen(CodeContext& context) const override;
};

class IsWall : public NBoolean
{
public:
    void CodeGen(CodeContext& context) const override;
};

