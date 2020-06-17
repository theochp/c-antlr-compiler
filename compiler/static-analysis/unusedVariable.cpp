#include "unusedVariable.h"

UnusedVariable::UnusedVariable(string nomVar, int lineNb, int charPosition) : nomVar(nomVar), lineNb(lineNb), charPosition(charPosition){
}

UnusedVariable::~UnusedVariable() {}

string UnusedVariable::getMessage() {
    string message = "Line "+to_string(lineNb)+" (position "+to_string(charPosition)+") :\n\t"+"Variable "+nomVar+" is never used.";
    return message;
}