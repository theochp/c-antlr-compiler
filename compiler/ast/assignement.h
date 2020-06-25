#pragma once

#include "operator.h"
#include "variable.h"

class Assignement : public Operator {
    Variable *lvalue;
    Expression *rvalue;

public:
    Assignement(Variable *lvalue, Expression *rvalue);
    ~Assignement() override;
    std::string print() override;
};