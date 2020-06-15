#pragma once

#include "expression.h"

class Constant : public Expression {
    int value;
public:
    Constant(int value);
};