#pragma once

#include <map>

#include "node.h"
#include "expression.h"

using namespace std;

class Affectation : public Node {
    map<string, Expression *> affectations;

public:
    void addAffectation(string, Expression *);
    virtual ~Affectation();
};
