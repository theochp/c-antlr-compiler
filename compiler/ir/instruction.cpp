#include <assert.h>

#include "instruction.h"

Instruction::Instruction(IROp op, const IRBlock *block) : block(block) {
    _op = op;
}

Instruction::Instruction(IROp op, string dest, const IRBlock *block) : block(block) {
    _op = op;
    _dest = dest;
}

Instruction::Instruction(IROp op, string dest, const vector<string>& operands, const IRBlock *block) : block(block) {
    _op = op;
    _dest = dest;
    
    copy(operands.begin(), operands.end(), back_inserter(_operands));
}

IROp Instruction::op() const {
    return _op;
}
string Instruction::dest() const {
    return _dest;
}

const vector<string>& Instruction::operands() {
    return _operands;
}

const string& Instruction::operand(int i) {
    assert(_operands.size() > i);
    return _operands[i];
}

const IRBlock *Instruction::getBlock() {
    return block;
}

std::ostream& operator<<(std::ostream& os, const Instruction& obj)
{
    /*switch (obj._op) {
        case cst:
            os << "movl $" << obj._source << ", %" << obj._dest;
            break;
        case store:
            os << "movl %" << obj._source << ", %" << obj._dest;
            break;
        case load:
            os << "movl %" << obj._source << ", %" << obj._dest;
            break;
        case ret:
            os << "movl %" << obj._source << ", %eax";
            break;
    }*/
    return os;
}