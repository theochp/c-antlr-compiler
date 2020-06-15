#include "expression.h"

Expression::Expression(Operator op, Expression *left, Expression *right)
    : op(op), left(left), right(right) {

}
Expression::~Expression() {
    delete left;
    delete right;
}
