#pragma once

#include "statement.h"

class Return : public Statement {

    Statement *statement;
public:
    Return();
    Return(Statement* statement);
    ~Return();
    const Statement *getStatement() const;
    virtual std::string print();
};