#pragma once

#include "statement.h"
#include "unoperator.h"

class UnExpression : public Statement {
    Statement *expr;
    UnOperator op;

public:
    UnExpression(UnOperator op, Statement *expr);
    ~UnExpression();
    const Statement *getExpr() const;
    const UnOperator& getOp() const;
    virtual std::string print();
};