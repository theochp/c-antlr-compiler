#pragma once
#include "error.h"

class VoidUsedAsRValue : public Error{

public:
    VoidUsedAsRValue(string nomFunc, int lineNb=0, int charPosition=0);
    ~VoidUsedAsRValue();
    string getMessage();
private:
    string nomFunc;
    int lineNb;
    int charPosition;
};
