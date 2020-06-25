#include "func.h"

string Func::getName() const {
    return name;
}

const Block *Func::getBlock() const {
    return block;
}

void Func::addParam(const FuncParam *param) {
    params.push_back(param);
}

const vector<const FuncParam*>& Func::getParams() const {
    return params;
}

Func::Func(string name, Block *block) 
    : name(name), block(block)
    {

}

Func::~Func() {
    delete block;
    for (auto param : params) {
        delete param;
    }
}