#pragma once

#include <vector>

#include "node.h"
#include "statement.h"

using namespace std;

class Block : public Node {
protected:
    vector<Statement *> statements;
    
public:
    void addStatement(Statement* statement);
    const vector<Statement *>& getStatements() const;
    virtual ~Block();
    virtual std::string print();
};