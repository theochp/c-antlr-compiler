#pragma once

#include <map>

#include "statement.h"
#include "expression.h"

using namespace std;

class Declaration : public Statement {
    map<string, pair<Expression *, int>> symbols;

public:
    void addSymbol(string, Expression *, int);
    const map<string, pair<Expression *, int>>& getSymbols() const;
    virtual ~Declaration();
    virtual std::string print();
};