#include "unusedVariable.h"

unusedVariable::unusedVariable(string nomVar, int lineNb, int charPosition) : nomVar(nomVar), lineNb(lineNb), charPosition(charPosition){
}

unusedVariable::~unusedVariable() {}

string unusedVariable::getMessage() {
    string message = "Line "+to_string(lineNb)+" (position "+to_string(charPosition)+") :\n\t"+"Variable "+nomVar+" is never used.";
    return message;
}