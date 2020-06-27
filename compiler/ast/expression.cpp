#include "expression.h"
#include <string>

Expression::Expression(Operator op, Statement *left, Statement *right, Statement *offset)
    : op(op), left(left), right(right), offset(offset){

}

Expression::Expression(Operator op, Statement *left, Statement *right)
    : op(op), left(left), right(right), offset(nullptr){

}


const Statement * Expression::getOffSet() const{
    return offset;
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
