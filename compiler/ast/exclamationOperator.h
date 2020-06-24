#pragma once
#include "statement.h"

class ExclamationOperator : public Statement{
    Statement *expr;
public:
    ExclamationOperator(Statement *expr);
    ~ExclamationOperator();
    const Statement *getExpr() const;
    virtual std::string print();
};