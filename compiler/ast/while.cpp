#include "while.h"

While::While(Block *block, Expression *condition)
    : block(block), condition(condition) {

}

While::~While() {
    delete block;
    delete condition;
}
const Block * While::getBlock() const {
    return block;
}
const Expression * While::getCondition() const {
    return condition;
}
