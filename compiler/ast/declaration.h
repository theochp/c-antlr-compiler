#pragma once

#include <map>

#include "statement.h"
#include "constant.h"

using namespace std;

class Declaration : public Statement {
    map<string, pair<Statement *, int>> symbols;

public:
    void addSymbol(string, Statement *, int);
    const map<string, pair<Statement *, int>>& getSymbols() const;
    virtual ~Declaration();
    virtual std::string print();
};