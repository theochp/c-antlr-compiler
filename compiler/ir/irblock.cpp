#include "irblock.h"

IRBlock::IRBlock(string label, IRFunc *func) : label(label), func(func) {
    
}

string IRBlock::getLabel() const {
    return label;
}

IRFunc *IRBlock::getFunc() const {
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

const IRBlock *IRBlock::getExitTrue() const {
    return exitTrue;
}

void IRBlock::setExitTrue(const IRBlock *exitTrue) {
    IRBlock::exitTrue = exitTrue;
}

const IRBlock *IRBlock::getExitFalse() const {
    return exitFalse;
}

void IRBlock::setExitFalse(const IRBlock *exitFalse) {
    IRBlock::exitFalse = exitFalse;
}

const string &IRBlock::getTestVarName() const {
    return testVarName;
}

void IRBlock::setTestVarName(const string &testVarName) {
    IRBlock::testVarName = testVarName;
}
