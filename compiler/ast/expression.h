#pragma once

#include "statement.h"
#include "operator.h"

class Expression : public Statement {
    Statement *left;
    Statement *right;
    Statement *offset;
    Operator op;

public:
    Expression(Operator op, Statement *left, Statement *right, Statement *Expression);
    Expression(Operator op, Statement *left, Statement *right);
    ~Expression();
    const Statement *getLeft() const;
    const Statement *getRight() const;
    const Operator& getOp() const;
    const Statement * getOffSet() const;
    virtual std::string print();
};