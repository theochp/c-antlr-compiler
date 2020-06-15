#pragma once

#include <vector>

#include "node.h"

using namespace std;

class Block : public Node {
    vector<Node *> nodes;
    
public:
    void addNode(Node* node);

    virtual ~Block();
};