#include "operator.h"
#include <string>

Operator::Operator(OpType op, Expression *left, Expression *right)
    : op(op), left(left), right(right) {

}

const Expression *Operator::getLeft() const {
    return left;
}

const Expression *Operator::getRight() const {
    return right;
}

const OpType& Operator::getOp() const {
    return op;
}

std::string Operator::print() {
    std::string res("(");
    res.append(left->print());
    res.append(" operator ");
    res.append(right->print());
    res.append(")");
    return res;
}

Operator::~Operator() {
    delete left;
    delete right;
}
