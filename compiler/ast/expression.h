#pragma once

#include "node.h"
#include "operator.h"

class Expression : public Node {
    Expression *left;
    Expression *right;
    Operator op;

public:
    Expression(Operator op, Expression *left, Expression *right);
    ~Expression();
};