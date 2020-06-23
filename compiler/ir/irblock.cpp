#include "irblock.h"

IRBlock::IRBlock(string label) : label(label) {
    
}

string IRBlock::getLabel() {
    return label;
}

const vector<Instruction*> &IRBlock::getInstructions() {
    return instructions;
}

void IRBlock::addInstruction(Instruction *instr) {
    if (instr != nullptr) {
        instructions.push_back(instr);
    }
}