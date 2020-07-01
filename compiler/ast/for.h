#pragma once

#include "statement.h"
#include "block.h"
#include "expression.h"

class For : public Statement {
    Block *block;
    vector<Expression *> expressions;
public:
    For(Block *block, vector<Expression *> expressions);
    ~For() override;
    const Block * getBlock() const;
    const vector<Expression *>& getExpressions() const;
};
