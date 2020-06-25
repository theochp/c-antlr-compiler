#pragma once

#include "statement.h"

class Expression : public Statement {
public:
    std::string print() override;
};