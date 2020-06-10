#include "instruction.h"

instruction::instruction(inst_type opType) {
    _op = opType;
}

instruction::instruction(inst_type opType, std::string source) {
    _op = opType;
    _source = source;
}

instruction::instruction(inst_type opType, std::string source, std::string dest) {
    _op = opType;
    _source = source;
    _dest = dest;
}

instruction::instruction(inst_type opType, std::string source, std::string dest, std::string operand) {
    _op = opType;
    _source = source;
    _dest = dest;
    _operand = operand;
}

inst_type instruction::op() {
    return _op;
}
string instruction::source() {
    return _source;
}
string instruction::dest() {
    return _dest;
}
string instruction::operand() {
    return _operand;
}


std::ostream& operator<<(std::ostream& os, const instruction& obj)
{
    switch (obj._op) {
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
    }
    return os;
}