#pragma once

#include "expression.h"
#include "variable.h"


class ArrayValue : public Expression {
    Variable * arrayBegin;
    Expression * offset;

public:
    ArrayValue(Variable * arrayBegin, Expression * offset);
    ~ArrayValue();
    virtual std::string print();
    const Variable& getArrayBegin();
    const Expression* getOffset();
};