#pragma once

#include "expression.h"

class Return : public Expression {

    Expression *statement;
public:
    Return();
    Return(Expression* statement);
    ~Return();
    const Expression *getStatement() const;
    virtual std::string print();
};