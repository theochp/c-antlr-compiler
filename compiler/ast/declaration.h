#pragma once

#include <map>

#include "statement.h"
#include "constant.h"

using namespace std;

class Declaration : public Statement {
    map<string, Constant *> symbols;

public:
    void addSymbol(string, Constant *);
    const map<string, Constant *>& getSymbols() const;
    virtual ~Declaration();
    virtual std::string print();
};