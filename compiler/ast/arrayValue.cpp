#include "arrayValue.h"

ArrayValue::ArrayValue(Variable * arrayBegin, Expression * offset) 
    : arrayBegin(arrayBegin), offset(offset){

}

ArrayValue::~ArrayValue(){

}

std::string ArrayValue::print(){
    std::string res("{");

    res.append(arrayBegin->print());
    res.append("[");
    res.append(offset->print());
    res.append("]");

    return res;
}