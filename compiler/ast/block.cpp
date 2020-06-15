#include "block.h"

void Block::addStatement(Statement *statement) {
    statements.push_back(statement);
}

Block::~Block() {
    for (auto it = statements.begin(); it != statements.end() ; ++it) {
        delete *it;
    }
}

const vector<Statement *>& Block::getStatements() const {
    return statements;
}

std::string Block::print() {
    std::string res("{");
    for (auto st = statements.begin(); st != statements.end(); ++st) {
        res.append("\t");
        res.append((*st)->print());
        res.append("\n");
    }
    res.append("}");
    return res;
}