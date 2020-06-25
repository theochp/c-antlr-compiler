#include "declaration.h"

void Declaration::addSymbol(string name, Expression * value) {
    symbols.emplace(name, value);
}

Declaration::~Declaration() {
    for (auto it = symbols.begin(); it != symbols.end(); ++it) {
        delete it->second;
    }
}

const map<string, Expression *>& Declaration::getSymbols() const {
    return symbols;
}

std::string Declaration::print() {
    std::string res("(");
    for (auto it = symbols.begin(); it != symbols.end(); ++it) {
        res.append((*it).first);
        res.append("=");
        if ((*it).second != nullptr) {   
            res.append((*it).second->print());
        } else {
            res.append("null");
        }
        res.append(",");
    }
    res.append(")");
    return res;
}