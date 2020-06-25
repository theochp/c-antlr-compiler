#pragma once
#include "statement.h"

class LogicalNot : public Statement{
    Statement *expr;
public:
    LogicalNot(Statement *expr);
    ~LogicalNot();
    const Statement *getExpr() const;
    virtual std::string print();
};