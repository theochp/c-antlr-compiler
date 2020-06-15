#pragma once

#include "expression.h"
#include "variable.h"

class Assignement : public Expression {
    Variable *lvalue;
    Expression *rvalue;

public:
    Assignement(Variable *lvalue, Expression *rvalue);
    ~Assignement();
};