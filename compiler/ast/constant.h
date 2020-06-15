#pragma once

#include "node.h"

class Constant : public Node {
    int value;
public:
    Constant(int value);
};