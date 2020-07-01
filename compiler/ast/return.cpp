#include "return.h"

Return::Return() : expression(nullptr) {

}
    
Return::Return(Expression* expression)
    : expression(expression) {

}

Return::~Return() {
    if (expression != nullptr) {
        delete expression;
    }
}

const Expression *Return::getExpression() const {
    return expression;
}

std::string Return::print() {
    std::string res("return(");
    res.append(expression->print());
    res.append(")");
    return res;
}