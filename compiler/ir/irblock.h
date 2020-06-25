#pragma once

#include <string>
#include <vector>

#include "instruction.h"
#include "irfunc.h"

using namespace std;

class IRFunc;
class Instruction;

class IRBlock {
    string label;
    vector<Instruction*> instructions;
    IRFunc *func;
    const IRBlock *exitTrue;
    const IRBlock *exitFalse;
    string testVarName;
public:
    IRBlock(string label, IRFunc *func);
    string getLabel() const;
    IRFunc *getFunc() const;
    void addInstruction(Instruction *instr);
    const vector<Instruction*> &getInstructions() const;

    const IRBlock *getExitTrue() const;

    void setExitTrue(const IRBlock *exitTrue);

    const IRBlock *getExitFalse() const;

    void setExitFalse(const IRBlock *exitFalse);

    const string &getTestVarName() const;

    void setTestVarName(const string &testVarName);
};
