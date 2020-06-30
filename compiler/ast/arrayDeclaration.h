#pragma once

#include "variable.h"
#include "statement.h"
#include <vector>

class ArrayDeclaration : public Statement{
    Variable * lvalue;
    std::vector<Statement *> expressions;
    std::vector<std::string> names;
    size_t size;

public:
    ArrayDeclaration();
    ArrayDeclaration(size_t size);
    ~ArrayDeclaration();
    void AddExpression(Statement * expr);
    const std::size_t& Size();
    void SetSize(size_t size);
    virtual std::string print();
    void AddNames(std::vector<std::string> names);
    void SetFirstName(std::string name);
    const std::vector<Statement *>& Expressions();
    const std::vector<std::string>& Names();
};