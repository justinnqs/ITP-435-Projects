#include "Node.h"
#include <sstream>

void NBlock::AddStatement(NStatement* statement)
{
	mStatements.emplace_back(statement);
}

void NBlock::CodeGen(CodeContext& context) const
{
	// TODO: Loop through statements in list and code gen them
    for (NStatement* statement : mStatements)
    {
        statement->CodeGen(context);
    }
    if(mMainBlock)
    {
        context.mOps.emplace_back("goto,1");
        context.mGoTos.insert(std::make_pair(context.mOps.size(),1));
    }
    if(mOptimize)
    {
        std::map<int,int>::iterator it1;
        for(it1 = context.mGoTos.begin(); it1 != context.mGoTos.end(); ++it1)
        {
            std::vector<int> changeGoTos;
            std::map<int,int>::iterator it2;
            int end = 0;
            changeGoTos.emplace_back(it1->first);
            it2 = context.mGoTos.find(it1->second);
            while(it2 != context.mGoTos.end())
            {
                changeGoTos.emplace_back(it2->first);
                end = it2->second;
                it2 = context.mGoTos.find(it2->second);
                
            }
            if(changeGoTos.size() > 1)
            {
                for(int i = 0; i < changeGoTos.size(); i++)
                {
                    context.mOps[changeGoTos[i]-1] = "goto," + std::to_string(end);
                }
            }
        }
    }
}

NNumeric::NNumeric(std::string& value)
{
	mValue = std::stoi(value);
}

NRotate::NRotate(NNumeric* dir)
	: mDir(dir)
{
}

void NRotate::CodeGen(CodeContext& context) const
{
	if (mDir->mValue == 0)
	{
		context.mOps.emplace_back("rotate,0");
	}
	else if (mDir->mValue == 1)
	{
		context.mOps.emplace_back("rotate,1");
	}
}

void NForward::CodeGen(CodeContext & context) const
{
    context.mOps.emplace_back("forward");
}

void NRangedAttack::CodeGen(CodeContext & context) const
{
    context.mOps.emplace_back("ranged_attack");

}

void NAttack::CodeGen(CodeContext & context) const
{
    context.mOps.emplace_back("attack");
}

NIfElse::NIfElse(NBoolean* boolVal, NBlock* ifBlock, NBlock* elseBlock)
    : mBoolVal(boolVal), mIfBlock(ifBlock), mElseBlock(elseBlock)
{
    
}

void NIfElse::CodeGen(CodeContext &context) const
{
    int je, go, ifLine, elseLine;
    mBoolVal->CodeGen(context);
    context.mOps.emplace_back("je,");
    je = context.mOps.size()-1;
    mElseBlock->CodeGen(context);
    elseLine = context.mOps.size();
    context.mOps.emplace_back("goto,");
    go = context.mOps.size()-1;
    mIfBlock->CodeGen(context);
    ifLine = context.mOps.size();
    
    context.mOps[je] = "je," + std::to_string(elseLine+2);
    context.mOps[go] = "goto," + std::to_string(ifLine+1);
    context.mGoTos.insert(std::make_pair(go+1, ifLine+1));
}

IsHuman::IsHuman(NNumeric* dir)
    : mDir(dir)
{
}

void IsHuman::CodeGen(CodeContext & context) const
{
    context.mOps.emplace_back("test_human," + std::to_string(mDir->mValue));
}

IsZombie::IsZombie(NNumeric* dir) : mDir(dir)
{
}

void IsZombie::CodeGen(CodeContext & context) const
{
    context.mOps.emplace_back("test_zombie," + std::to_string(mDir->mValue));
}

void IsPassable::CodeGen(CodeContext & context) const
{
    context.mOps.emplace_back("test_passable");
}

void IsRandom::CodeGen(CodeContext & context) const
{
    context.mOps.emplace_back("test_random");
}

void IsWall::CodeGen(CodeContext & context) const
{
    context.mOps.emplace_back("test_wall");
}

