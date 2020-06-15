#include "expression.h"

Expression::Expression(Operator op, Statement *left, Statement *right)
    : op(op), left(left), right(right) {

}
Expression::~Expression() {
    delete left;
    delete right;
}
