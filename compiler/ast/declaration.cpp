#include "declaration.h"

void Declaration::addSymbol(string name, Constant * value) {
    symbols.emplace(name, value);
}

Declaration::~Declaration() {
    for (auto it = symbols.begin(); it != symbols.end(); ++it) {
        delete it->second;
    }
}