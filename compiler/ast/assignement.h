#pragma once

#include "operator.h"
#include "variable.h"
#include "expression.h"

class Assignement : public Operator {
    Variable *lvalue;
    Expression *offset;
    Expression *rvalue;

public:
    Assignement(Variable *lvalue, Expression *rvalue, Expression *offset);
    Assignement(Variable *lvalue, Expression *rvalue);
    ~Assignement();
    virtual std::string print();
};