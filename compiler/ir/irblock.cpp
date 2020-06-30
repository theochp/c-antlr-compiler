#include "irblock.h"

IRBlock::IRBlock(string label, const IRFunc *func) : label(label), func(func) {
    
}

string IRBlock::getLabel() const {
    return label;
}

const IRFunc *IRBlock::getFunc() const {
    return func;
}

const vector<Instruction*> &IRBlock::getInstructions() const {
    return instructions;
}

void IRBlock::addInstruction(Instruction *instr) {
    if (instr != nullptr) {
        instructions.push_back(instr);
    }
}