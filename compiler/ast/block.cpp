#include "block.h"

void Block::addNode(Node *node) {
    nodes.push_back(node);
}

Block::~Block() {
    for (auto it = nodes.begin(); it != nodes.end() ; ++it) {
        delete *it;
    }
}