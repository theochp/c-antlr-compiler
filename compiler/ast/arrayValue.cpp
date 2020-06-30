#include "arrayValue.h"

ArrayValue::ArrayValue(Variable * arrayBegin, Expression * offset) 
    : arrayBegin(arrayBegin), offset(offset){

}

ArrayValue::~ArrayValue(){
    delete arrayBegin;
    delete offset;
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

const Expression * ArrayValue::getOffset(){
    return offset;
}