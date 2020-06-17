#pragma once

#include "error.h"

class UndeclaredVariable : public Error{

public:
    UndeclaredVariable(string nomVar, int lineNb=0, int charPosition=0);
    ~UndeclaredVariable();
    string getMessage();
private:
    string nomVar;
    int lineNb;
    int charPosition;
};
