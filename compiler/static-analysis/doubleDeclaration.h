#pragma once
#include <exception>
using namespace std;

#include "error.h"

class DoubleDeclaration : public Error {

public:
    DoubleDeclaration(string nomVar, int lineNb=0, int charPosition=0);
    ~DoubleDeclaration();
    string getMessage();
private:
    string nomVar;
    int lineNb;
    int charPosition;
};


