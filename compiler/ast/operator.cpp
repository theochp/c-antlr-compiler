#include "operator.h"

Operator::Operator (const OpType& opType) 
    : opType(opType) {

}

Operator& Operator::operator=(const OpType& opType) {
    this->opType = opType;

    return *this;
}

bool Operator::operator ==(const OpType &opType) const {
    return this->opType == opType;
}
