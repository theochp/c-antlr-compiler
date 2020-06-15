#pragma once

#include <map>

#include "statement.h"
#include "constant.h"

using namespace std;

class Declaration : public Statement {
    map<string, Constant *> symbols;

public:
    void addSymbol(string, Constant *);
    virtual ~Declaration();
    virtual std::string print();
};