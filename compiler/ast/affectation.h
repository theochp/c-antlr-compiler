#pragma once

#include <map>

#include "node.h"

using namespace std;

class Affectation : public Node {
    map<string, Expression *> affectations;

public:
    void addSymbol(string, Expression *);
    virtual ~Affectation();
};
