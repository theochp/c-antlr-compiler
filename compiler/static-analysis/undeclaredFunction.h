#pragma once

#include "error.h"

class UndeclaredFunction : public Error{

public:
    UndeclaredFunction(string nomFunc, int lineNb=0, int charPosition=0);
    ~UndeclaredFunction();
    string getMessage();
private:
    string nomFunc;
    int lineNb;
    int charPosition;
};
