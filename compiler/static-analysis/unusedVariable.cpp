#include "unusedVariable.h"

UnusedVariable::UnusedVariable(string nomVar, int lineNb, int charPosition) : nomVar(nomVar), lineNb(lineNb), charPosition(charPosition){
}

UnusedVariable::~UnusedVariable() {}

string UnusedVariable::getMessage() {
    string message = AS_COMMENT+"Line "+to_string(lineNb)+" (position "+to_string(charPosition)+") :\n"+AS_COMMENT+"\t"+"Variable "+nomVar+" is never used.";
    return message;
}