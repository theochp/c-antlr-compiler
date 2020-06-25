#include "block.h"

void Block::addStatement(Statement *statement) {
    statements.push_back(statement);
}

Block::~Block() {
    for (auto & statement : statements) {
        delete statement;
    }
}

const vector<Statement *>& Block::getStatements() const {
    return statements;
}

std::string Block::print() {
    std::string res("{");
    for (auto & statement : statements) {
        res.append("\t");
        res.append(statement->print());
        res.append("\n");
    }
    res.append("}");
    return res;
}