#pragma once

#include <string>
#include <vector>

#include "statement.h"

using namespace std;

class FuncCall : public Statement {
    string name;
    vector<Statement*> paramStatements;

public:
    FuncCall(string name);
    ~FuncCall();
    string getName() const;
    const vector<Statement*>& getParamStatements() const;
    void addParamStatement(Statement *param);
    void addParamStatements(const vector<Statement *>& statements);
};