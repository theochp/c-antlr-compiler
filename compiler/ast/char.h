#pragma once

#include "expression.h"

class Char : public Expression {
    int value;
public:
    Char(int value);
    int getValue () const;
    virtual std::string print();
};
