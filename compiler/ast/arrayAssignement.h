#pragma once

#include "statement.h"
#include "variable.h"

class ArrayAssignement : public Statement{
    Variable * lvalue;
    std::string values[];

public:
    ArrayAssignement(Variable *lvalue, std::string values[]);
    ~ArrayAssignement();
    virtual std::string print();
};