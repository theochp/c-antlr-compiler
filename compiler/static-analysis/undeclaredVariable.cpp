#include "undeclaredVariable.h"

UndeclaredVariable::UndeclaredVariable(string nomVar, int lineNb, int charPosition) {
    this->nomVar = nomVar;
    this->lineNb = lineNb;
    this->charPosition = charPosition;
}

string UndeclaredVariable::getMessage() {
    string message = "Line "+to_string(lineNb)+" (position "+to_string(charPosition)+") :\n\t"+"Variable "+nomVar+" was not declared.";
    return message;
}

UndeclaredVariable::~UndeclaredVariable() {}