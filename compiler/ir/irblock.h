#pragma once

#include <string>
#include <vector>

#include "instruction.h"

using namespace std;

class IRBlock {
    string label;
    vector<Instruction*> instructions;
public:
    IRBlock(string label);
    string getLabel();
    void addInstruction(Instruction *instr);
    const vector<Instruction*> &getInstructions();
};
