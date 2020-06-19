#pragma once

#include "warning.h"

class UnusedVariable : public Warning {
public:
    UnusedVariable(string nomVar, int lineNb=0, int charPosition=0);
    ~UnusedVariable();
    string getMessage();
private:
    string nomVar;
    int lineNb;
    int charPosition;
};
