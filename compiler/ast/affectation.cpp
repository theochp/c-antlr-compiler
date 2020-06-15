#include "affectation.h"

void Affectation::addAffectation(string name, Expression * expr) {
    affectations.emplace(name, expr);
}

Affectation::~Affectation() {
    for (auto it = affectations.begin(); it != affectations.end(); ++it) {
        delete it->second;
    }
}
