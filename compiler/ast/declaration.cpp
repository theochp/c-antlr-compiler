#include "declaration.h"

void Declaration::addSymbol(string name, Statement * value, int size) {
    symbols.emplace(name, make_pair(value, size));
}

Declaration::~Declaration() {
    for (auto it = symbols.begin(); it != symbols.end(); ++it) {
        delete it->second.first;
    }
}

const map<string, pair<Statement *, int>>& Declaration::getSymbols() const {
    return symbols;
}

std::string Declaration::print() {
    std::string res("(");
    for (auto it = symbols.begin(); it != symbols.end(); ++it) {
        res.append((*it).first);
        res.append("=");
        if ((*it).second.first != nullptr) {   
            res.append((*it).second.first->print());
        } else {
            res.append("null");
        }
        res.append(",");
    }
    res.append(")");
    return res;
}