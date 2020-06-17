#include "func.h"

string Func::getName() const {
    return name;
}

const Block *Func::getBlock() const {
    return block;
}

Func::Func(string name, Block *block) 
    : name(name), block(block)
    {

}

Func::~Func() {
    delete block;
}