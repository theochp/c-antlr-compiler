#include "undeclaredVariable.h"

undeclaredVariable::undeclaredVariable(string nomVar, int lineNb, int charPosition) {
    this->nomVar = nomVar;
    this->lineNb = lineNb;
    this->charPosition = charPosition;
}

string undeclaredVariable::getMessage() {
    string message = "Line "+to_string(lineNb)+" (position "+to_string(charPosition)+") :\n\t"+"Variable "+nomVar+" was not declared.";
    return message;
}

undeclaredVariable::~undeclaredVariable() {}