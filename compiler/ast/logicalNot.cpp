#include "logicalNot.h"

#include <string>

LogicalNot::LogicalNot(Expression *expr) : expr(expr) { }

const Expression *LogicalNot::getExpr() const {
    return expr;
}

std::string LogicalNot::print() {
    std::string res("(");
    res.append("!");
    res.append(expr->print());
    res.append(")");
    return res;
}

LogicalNot::~LogicalNot() {
    delete expr;
}