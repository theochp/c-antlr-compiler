#pragma once

#include "node.h"
#include "block.h"
#include "funcparam.h"

class Func : public Node {
    string name;
    Block *block;
    vector<const FuncParam*> params;
public:
    string getName() const;
    const Block *getBlock() const;
    void addParam(const FuncParam *param);
    const vector<const FuncParam*>& getParams() const;

    Func(string name, Block *block);
    ~Func();
};