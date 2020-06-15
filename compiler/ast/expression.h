#pragma once

#include "node.h"
#include "operator.h"

class Expression : public Node {
    Node *left;
    Node *right;
    Operator op;
};