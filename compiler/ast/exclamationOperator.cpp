#include "exclamationOperator.h"

#include <string>

ExclamationOperator::ExclamationOperator(Statement *expr) : expr(expr) { }

const Statement *ExclamationOperator::getExpr() const {
    return expr;
}

std::string ExclamationOperator::print() {
    std::string res("(");
    res.append("!");
    res.append(expr->print());
    res.append(")");
    return res;
}

ExclamationOperator::~ExclamationOperator() {
    delete expr;
}