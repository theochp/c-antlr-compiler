#pragma once

#include "expression.h"

class Return : public Statement {

    Expression *expression;
public:
    Return();
    Return(Expression* expression);
    ~Return();
    const Expression *getExpression() const;
    virtual std::string print();
};