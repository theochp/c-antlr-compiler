#include "doubleDeclaration.h"

doubleDeclaration::doubleDeclaration(string nomVar, int lineNb, int charPosition) {
    this->nomVar = nomVar;
    this->lineNb = lineNb;
    this->charPosition = charPosition;
}

string doubleDeclaration::getMessage() {
    string message = "Line "+to_string(lineNb)+" (position "+to_string(charPosition)+") :\n\t"+"Variable "+nomVar+" was declared twice.";
    return message;
}

doubleDeclaration::~doubleDeclaration() {}