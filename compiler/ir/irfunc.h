#pragma once

#include <string>
#include <vector>

#include "irblock.h"

using namespace std;

class IRBlock;

class IRFunc {
    string name;
    vector<IRBlock *> blocks;
    vector<string> params;
public:
    IRFunc(string name);
    string getName() const;
    const vector<IRBlock *>& getBlocks() const;
    void addBlock(IRBlock *block);
    void addParam(string param);
    const vector<string>& getParams() const;
    ~IRFunc();
};