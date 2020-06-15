#include "expression.h"

Expression::Expression(Node *nodeLeft, Node *nodeRight, Operator ope) : op(ope) {
    left = nodeLeft;
    right = nodeRight;
}

void Expression::addLeftNode(Node *node) {
    left = node;
}

void Expression::addRightNode(Node *node) {
    right = node;
}

void Expression::addOperator(Operator ope) {
    op = ope;
}

Expression::~Expression() {
    delete left;
    delete right;
}
