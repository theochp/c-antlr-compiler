#pragma once

#include <string>

#include "expression.h"

using namespace std;

class Variable : public Expression {
    string name;

public:
    Variable(string name);
    const string& getName() const;
    virtual std::string print();
};