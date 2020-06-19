#include "instruction.h"

Instruction::Instruction(inst_type opType) {
    _op = opType;
}

Instruction::Instruction(inst_type opType, std::string source) {
    _op = opType;
    _source = source;
}

Instruction::Instruction(inst_type opType, std::string source, std::string dest) {
    _op = opType;
    _source = source;
    _dest = dest;
}

Instruction::Instruction(inst_type opType, std::string source, std::string dest, std::string operand) {
    _op = opType;
    _source = source;
    _dest = dest;
    _operand = operand;
}

inst_type Instruction::op() const {
    return _op;
}
string Instruction::source() const {
    return _source;
}
string Instruction::dest() const {
    return _dest;
}
string Instruction::operand() const {
    return _operand;
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