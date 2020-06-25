#pragma once

#include "expression.h"
#include "variable.h"

class Assignement : public Expression {
    Variable *lvalue;
    Expression *offset;
    Statement *rvalue;

public:
    Assignement(Variable *lvalue, Statement *rvalue, Expression *offset);
    Assignement(Variable *lvalue, Statement *rvalue);
    ~Assignement();
    virtual std::string print();
};