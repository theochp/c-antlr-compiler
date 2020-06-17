#include "doubleDeclaration.h"

DoubleDeclaration::DoubleDeclaration(string nomVar, int lineNb, int charPosition) {
    this->nomVar = nomVar;
    this->lineNb = lineNb;
    this->charPosition = charPosition;
}

string DoubleDeclaration::getMessage() {
    string message = "Line "+to_string(lineNb)+" (position "+to_string(charPosition)+") :\n\t"+"Variable "+nomVar+" was declared twice.";
    return message;
}

DoubleDeclaration::~DoubleDeclaration() {}