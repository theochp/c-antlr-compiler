#include "expression.h"
#include <string>

Expression::Expression(Operator op, Statement *left, Statement *right)
    : op(op), left(left), right(right) {

}

const Statement *Expression::getLeft() const {
    return left;
}

const Statement *Expression::getRight() const {
    return right;
}

const Operator& Expression::getOp() const {
    return op;
}

std::string Expression::print() {
    std::string res("(");
    res.append(left->print());
    res.append(" "+op.print()+" ");
    res.append(right->print());
    res.append(")");
    return res;
}

Expression::~Expression() {
    delete left;
    delete right;
}
