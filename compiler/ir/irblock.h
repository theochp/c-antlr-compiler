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
    const IRFunc *func;
public:
    IRBlock(string label, const IRFunc *func);
    string getLabel() const;
    const IRFunc *getFunc() const;
    void addInstruction(Instruction *instr);
    const vector<Instruction*> &getInstructions() const;
};
