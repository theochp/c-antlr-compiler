#pragma once

#include "statement.h"
#include "operator.h"

class Expression : public Statement {
    Statement *left;
    Statement *right;
    Operator op;

public:
    Expression(Operator op, Statement *left, Statement *right);
    ~Expression();
    virtual std::string print();
};