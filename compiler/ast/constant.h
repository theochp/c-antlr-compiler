#pragma once

#include "expression.h"

class Constant : public Expression {
    int value;
public:
    explicit Constant(int value);
    int getValue() const;
    std::string print() override;
};