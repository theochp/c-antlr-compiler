#include "undeclaredFunction.h"

UndeclaredFunction::UndeclaredFunction(string nomFunc, int lineNb, int charPosition) {
    this->nomFunc = nomFunc;
    this->lineNb = lineNb;
    this->charPosition = charPosition;
}

string UndeclaredFunction::getMessage() {
    string message = AS_COMMENT+"Line "+to_string(lineNb)+" (position "+to_string(charPosition)+") :\n"+AS_COMMENT+"\t"+"Function "+nomFunc+" was not declared.";
    return message;
}

UndeclaredFunction::~UndeclaredFunction() {}