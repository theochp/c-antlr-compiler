#pragma once

#include "statement.h"
#include "variable.h"
#include <vector>

class ArrayDeclaration : public Statement{
    Variable * lvalue;
    std::vector<std::string> values;
    std::vector<std::string> names;
    size_t size;

public:
    ArrayDeclaration();
    ArrayDeclaration(size_t size);
    ~ArrayDeclaration();
    void AddValue(string value);
    const std::size_t& Size();
    void SetSize(size_t size);
    virtual std::string print();
    void AddNames(std::vector<std::string> names);
    void SetFirstName(std::string name);
    const std::vector<std::string>& Values();
    const std::vector<std::string>& Names();
};