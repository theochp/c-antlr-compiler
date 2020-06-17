#pragma once

#include <string>
#include <vector>

#include "instruction.h"

using namespace std;

class Block {
    string label;
    vector<Instruction*> instructions;
public:
    Block(string label);
    void addInstruction(Instruction *instr);
};
