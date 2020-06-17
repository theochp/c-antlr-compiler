#include "doubleDeclaration.h"

doubleDeclaration::doubleDeclaration(string nomVar, int lineNb) {
    this->nomVar = nomVar;
    this->lineNb = lineNb;
}

string doubleDeclaration::getMessage() {
    string message = "Line "+to_string(lineNb)+":\n"+"Variable "+nomVar+" was declared twice.";
    return message;
}

doubleDeclaration::~doubleDeclaration() {}