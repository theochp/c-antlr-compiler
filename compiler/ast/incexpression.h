#pragma once

#include "statement.h"
#include "unoperator.h"
#include "variable.h"

class IncExpression : public Expression{
    UnOpType optype;
    Variable *variable;
    std::string dest;

public:
    IncExpression(UnOpType optype, Variable* variable, std::string dest);
    ~IncExpression();
    UnOpType & getOptype();
    Variable * getVariable();
    std::string getDest();
};