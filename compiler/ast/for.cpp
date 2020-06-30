#include "for.h"

For::For(Block *block, vector<Expression*> expressions)
    : block(block), expressions(std::move(expressions)) {

}

For::~For() {
    delete block;
    for (auto expr : expressions) {
        delete expr;
    }
}
const Block * For::getBlock() const {
    return block;
}
const vector<Expression*>& For::getExpressions() const {
    return expressions;
}
