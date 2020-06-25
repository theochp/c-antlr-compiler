#include "assignement.h"

Assignement::Assignement(Variable *lvalue, Expression *rvalue)
    : Operator(ASSIGN, lvalue, rvalue), lvalue(lvalue), rvalue(rvalue) {
}

Assignement::~Assignement() {
    delete lvalue;
    delete rvalue;
}

std::string Assignement::print() {
    return Operator::print();
}