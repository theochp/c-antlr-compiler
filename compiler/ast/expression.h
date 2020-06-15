#pragma once

#include "node.h"
#include "operator.h"

class Expression : public Node {
    Node *left;
    Node *right;
    Operator op;
public:
    Expression(Node* nodeLeft, Node* nodeRight, Operator op);
    void addLeftNode(Node* node);
    void addRightNode(Node* node);
    void addOperator(Operator op);
};