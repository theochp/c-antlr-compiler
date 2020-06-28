#include "unoperator.h"

UnOperator::UnOperator (const UnOpType& opType) 
    : opType(opType) {

}

const UnOpType& UnOperator::type() const {
    return opType;
}

std::string UnOperator::print() {
    switch(opType) {
        case UN_PLUS:
            return "+";
        case UN_MINUS:
            return "-";
        case BITWISE_NOT:
            return "~";
        default:
            return "";
    }
}

UnOperator& UnOperator::operator=(const UnOpType& opType) {
    this->opType = opType;

    return *this;
}

bool UnOperator::operator ==(const UnOpType &opType) const {
    return this->opType == opType;
}
