#pragma once

#include "node.h"
#include "block.h"

class Func : public Node {
    string name;
    Block *block;
    
public:
    string getName() const;
    const Block *getBlock() const;

    Func(string name, Block *block);
    ~Func();
};