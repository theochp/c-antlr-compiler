#pragma once

#include "statement.h"
#include "block.h"
#include "expression.h"

class While : public Statement {
    Block *block;
    Expression *condition;
public:
    While(Block *block, Expression *condition);
    ~While() override;
    const Block * getBlock() const;
    const Expression * getCondition() const;
};
