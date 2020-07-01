#pragma once

#include "statement.h"
#include "expression.h"
#include "block.h"

class IfElse : public Statement {
    Block *ifBlock;
    Block *elseBlock;
    Expression *condition;
public:
    IfElse(Block *ifBlock, Block *elseBlock, Expression *condition);
    ~IfElse() override;
    const Block * getIfBlock() const;
    const Block * getElseBlock() const;
    const Expression * getCondition() const;
};