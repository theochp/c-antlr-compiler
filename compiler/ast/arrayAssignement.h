#pragma once

#include "statement.h"
#include "variable.h"
#include <vector>

class ArrayAssignement : public Statement{
    Variable * lvalue;
    std::vector<std::string> values;

public:
    ArrayAssignement();
    ~ArrayAssignement();
    void AddValue(string value);
    std::size_t Size();
    virtual std::string print();
};