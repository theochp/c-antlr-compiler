#include "block.h"

void Block::addStatement(Statement *statement) {
    statements.push_back(statement);
}

Block::~Block() {
    for (auto it = statements.begin(); it != statements.end() ; ++it) {
        delete *it;
    }
}