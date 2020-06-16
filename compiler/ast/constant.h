#pragma once

#include "statement.h"

class Constant : public Statement {
    int value;
public:
    Constant(int value);
    int getValue() const;
    virtual std::string print();
};