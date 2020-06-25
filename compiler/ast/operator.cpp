#include "operator.h"

Operator::Operator (const OpType& opType) 
    : opType(opType) {

}

const OpType& Operator::type() const {
    return opType;
}

std::string Operator::print() {
    switch(opType) {
        case ADD:
            return "+";
        case MINUS:
            return "-";
        case MULT:
            return "*";
        case DIV:
            return "/";
        case ASSIGN:
            return "=";
        case INFCOMP:
            return "<";
        case SUPCOMP:
            return ">";
        case EQUALCOMP:
            return "==";
        case DIFFCOMP:
            return "!=";
        default: return "";
    }
}

Operator& Operator::operator=(const OpType& opType) {
    this->opType = opType;

    return *this;
}

bool Operator::operator ==(const OpType &opType) const {
    return this->opType == opType;
}
