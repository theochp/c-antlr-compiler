#include "unexpression.h"
#include <string>

UnExpression::UnExpression(UnOperator op, Statement *expr)
    : op(op), expr(expr) {

}

const Statement *UnExpression::getExpr() const {
    return expr;
}

const UnOperator& UnExpression::getOp() const {
    return op;
}

std::string UnExpression::print() {
    std::string res("(");
    res.append(" "+op.print()+" ");
    res.append(expr->print());
    res.append(")");
    return res;
}

UnExpression::~UnExpression() {
    delete expr;
}
