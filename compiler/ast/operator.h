#pragma once

#include "expression.h"
#include "operator.h"

typedef enum {
    MULT,
    DIV,
    ADD,
    MINUS,
    ASSIGN,
    INFCOMP,
    INFEQCOMP,
    SUPCOMP,
    SUPEQCOMP,
    EQUALCOMP,
    DIFFCOMP,
    BITWISE_AND,
    BITWISE_OR,
    BITWISE_XOR
} OpType;

class Operator : public Expression {
    Expression *left;
    Expression *right;
    OpType op;

public:
    Operator(OpType op, Expression *left, Expression *right);
    ~Operator() override;
    const Expression *getLeft() const;
    const Expression *getRight() const;
    const OpType& getOp() const;
    std::string print() override;
};