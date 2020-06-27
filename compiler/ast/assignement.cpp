#include "assignement.h"

Assignement::Assignement(Variable *lvalue, Statement *rvalue, Statement * offset) 
    : Expression(ASSIGN, lvalue, rvalue, offset) {
}

Assignement::Assignement(Variable *lvalue, Statement *rvalue) 
    : Expression(ASSIGN, lvalue, rvalue) {
}

Assignement::~Assignement() {
    delete lvalue;
    delete rvalue;
}

std::string Assignement::print() {
    return Expression::print();
}