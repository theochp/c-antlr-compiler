#pragma once

#include <map>

#include "expression.h"
#include "constant.h"

using namespace std;

class Declaration : public Statement {
    map<string, Expression *> symbols;

public:
    void addSymbol(string, Expression *);
    const map<string, Expression *>& getSymbols() const;
    virtual ~Declaration();
    virtual std::string print();
};