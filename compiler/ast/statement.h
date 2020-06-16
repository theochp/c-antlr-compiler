#pragma once

#include "node.h"

class Statement : public Node {
public:
    virtual std::string print();
};