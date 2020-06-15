#pragma once

#include <vector>

#include "node.h"
#include "statement.h"

using namespace std;

class Block : public Node {
    vector<Statement *> statements;
    
public:
    void addStatement(Statement* statement);
    virtual ~Block();
};