#pragma once
#include "expression.h"

class LogicalNot : public Expression{
    Expression *expr;
public:
    LogicalNot(Expression *expr);
    ~LogicalNot();
    const Expression *getExpr() const;
    virtual std::string print();
};