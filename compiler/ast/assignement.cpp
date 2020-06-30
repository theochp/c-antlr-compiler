#include "assignement.h"

Assignement::Assignement(Variable *lvalue, Expression *rvalue, Expression * offset) 
    : Operator (ASSIGN, lvalue, rvalue, offset) {
}

Assignement::Assignement(Variable *lvalue, Expression *rvalue) 
    : Operator (ASSIGN, lvalue, rvalue) {
}

Assignement::~Assignement() {
    delete lvalue;
    delete rvalue;
    delete offset;
}

std::string Assignement::print() {
    return Operator::print();
}