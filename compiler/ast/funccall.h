#pragma once

#include <string>
#include <vector>

#include "expression.h"

using namespace std;

class FuncCall : public Expression {
    string name;
    vector<Expression*> paramStatements;

public:
    FuncCall(string name);
    ~FuncCall();
    string getName() const;
    const vector<Expression*>& getParamStatements() const;
    void addParamStatement(Expression *param);
    void addParamStatements(const vector<Expression *>& statements);
};