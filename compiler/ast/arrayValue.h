#pragma once

#include "expression.h"
#include "variable.h"


class ArrayValue : public Statement{
    Variable * arrayBegin;
    Statement * offset;

public:
    ArrayValue(Variable * arrayBegin, Statement * offset);
    ~ArrayValue();
    virtual std::string print();
    const Variable& getArrayBegin();
    const Statement* getOffset();
};