#include "block.h"

void Block::addInstruction(Instruction *instr) {
    if (instr != nullptr) {
        instructions.push_back(instr);
    }
}