#pragma once

#include <map>

#include "node.h"
#include "constant.h"

using namespace std;

class Declaration : public Node {
    map<string, Constant *> symbols;

public:
    void addSymbol(string, Constant *);
    virtual ~Declaration();
};