#pragma once

#include <map>

#include "statement.h"
#include "constant.h"

using namespace std;

class Declaration : public Statement {
    map<string, Statement *> symbols;

public:
    void addSymbol(string, Statement *);
    const map<string, Statement *>& getSymbols() const;
    virtual ~Declaration();
    virtual std::string print();
};