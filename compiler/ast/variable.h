#pragma once

#include <string>

#include "node.h"

using namespace std;

class Variable : public Node {
    string name;

public:
    Variable(string name);
};