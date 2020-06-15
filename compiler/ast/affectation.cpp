#include "affectation.h"

void Affectation::addSymbol(string name, Expression * expr) {
    symbols.emplace(name, expr);
}

Affectation::~Affectation() {
    for (auto it = symbols.begin(); it != symbols.end(); ++it) {
        delete it->second;
    }
}
