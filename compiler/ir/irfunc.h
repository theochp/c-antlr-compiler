#pragma once

#include <string>
#include <vector>

#include "irblock.h"

using namespace std;

class IRBlock;

class IRFunc {
    string name;
    vector<const IRBlock *> blocks;
    vector<string> params;
    string returnValueLoc;
public:
    IRFunc(string name);
    string getName() const;
    const vector<const IRBlock *>& getBlocks() const;
    void addBlock(const IRBlock *block);
    void addParam(string param);
    const vector<string>& getParams() const;

    const string &getReturnValueLoc() const;

    void setReturnValueLoc(const string &returnValueLoc);

    ~IRFunc();
};