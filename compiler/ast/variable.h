#pragma once

#include <string>

#include "statement.h"

using namespace std;

class Variable : public Statement {
    string name;

public:
    Variable(string name);
    virtual std::string print();
};