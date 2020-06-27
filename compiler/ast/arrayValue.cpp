#include "arrayValue.h"

ArrayValue::ArrayValue(Variable * arrayBegin, Statement * offset) 
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


const Variable& ArrayValue::getArrayBegin(){
    return *arrayBegin;
}

const Statement * ArrayValue::getOffset(){
    return offset;
}