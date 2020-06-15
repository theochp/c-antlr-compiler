#include "assignement.h"

Assignement::Assignement(Variable *lvalue, Statement *rvalue) 
    : Expression(ASSIGN, lvalue, rvalue) {
}

Assignement::~Assignement() {
    delete lvalue;
    delete rvalue;
}