#include "voidUsedAsRValue.h"

VoidUsedAsRValue::VoidUsedAsRValue(string nomFunc, int lineNb, int charPosition) {
    this->nomFunc = nomFunc;
    this->lineNb = lineNb;
    this->charPosition = charPosition;
}

string VoidUsedAsRValue::getMessage() {
    string message = AS_COMMENT+"Line "+to_string(lineNb)+" (position "+to_string(charPosition)+") :\n"+AS_COMMENT+"\t"+"Function "+nomFunc+" has a returned type void and can't be used has rvalue.";
    return message;
}

VoidUsedAsRValue::~VoidUsedAsRValue() {}