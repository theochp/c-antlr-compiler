#pragma once

#include "expression.h"
#include "variable.h"

class Assignement : public Expression {
    Variable *lvalue;
    Statement *offset;
    Statement *rvalue;

public:
    Assignement(Variable *lvalue, Statement *rvalue, Statement *offset);
    Assignement(Variable *lvalue, Statement *rvalue);
    ~Assignement();
    virtual std::string print();
};