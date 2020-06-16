#pragma once

#include "statement.h"

class Return : public Statement {

    Statement *statement;
public:
    Return();
    Return(Statement* statement);
    const Statement *getStatement() const;
    virtual std::string print();
};