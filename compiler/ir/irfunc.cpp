#include "irfunc.h"

string IRFunc::getName() const {
    return name;
}

const vector<const IRBlock *>& IRFunc::getBlocks() const {
    return blocks;
}

void IRFunc::addBlock(const IRBlock *block) {
    blocks.push_back(block);
}

void IRFunc::addParam(string param) {
    params.push_back(param);
}

const vector<string>& IRFunc::getParams() const {
    return params;
}

IRFunc::IRFunc(string name) : name(name) {
    
}

IRFunc::~IRFunc() {
    for (auto b : blocks) {
        delete b;
    }
}
