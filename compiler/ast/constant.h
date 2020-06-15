#pragma once

#include "statement.h"

class Constant : public Statement {
    int value;
public:
    Constant(int value);
};