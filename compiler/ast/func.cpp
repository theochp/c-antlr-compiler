#include "func.h"

string Func::getName() const {
    return name;
}

const Block *Func::getBlock() const {
    return block;
}

void Func::setBlock(Block * block) {
    this->block = block;
}

void Func::addParam(const FuncParam *param) {
    params.push_back(param);
}

const vector<const FuncParam*>& Func::getParams() const {
    return params;
}

Func::Func(string name) 
    : name(name)
    {

}

Func::~Func() {
    delete block;
    for (auto param : params) {
        delete param;
    }
}