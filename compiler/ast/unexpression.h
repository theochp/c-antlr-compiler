#pragma once

#include "expression.h"
#include "unoperator.h"

class UnExpression : public Expression {
    Expression *expr;
    UnOperator op;

public:
    UnExpression(UnOperator op, Expression *expr);
    ~UnExpression();
    const Expression *getExpr() const;
    const UnOperator& getOp() const;
    virtual std::string print();
};