#include "operator.h"

Operator::Operator(OpType o) {
    opType = o;
}

OpType Operator::getType() {
    return opType;
}