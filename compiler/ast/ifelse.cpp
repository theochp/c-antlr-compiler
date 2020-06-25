#include "ifelse.h"

IfElse::IfElse(Block *ifBlock, Block *elseBlock, Expression *condition)
    : ifBlock(ifBlock), elseBlock(elseBlock), condition(condition){

}

IfElse::~IfElse() {
    delete ifBlock;
    if (elseBlock != nullptr) {
        delete elseBlock;
    }
    delete condition;
}

const Block * IfElse::getIfBlock() const {
    return ifBlock;
}

const Block * IfElse::getElseBlock() const {
    return elseBlock;
}

const Expression * IfElse::getCondition() const {
    return condition;
}
