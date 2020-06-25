#pragma once

#include "statement.h"
#include "variable.h"
#include <vector>

class ArrayDeclaration : public Statement{
    Variable * lvalue;
    std::vector<std::string> values;
    std::vector<std::string> names;

public:
    ArrayDeclaration();
    ~ArrayDeclaration();
    void AddValue(string value);
    std::size_t Size();
    virtual std::string print();
    void SetNames(std::vector<std::string> names);
    const std::vector<std::string>& Values();
    const std::vector<std::string>& Names();
};