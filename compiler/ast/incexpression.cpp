#include "incexpression.h"


IncExpression::IncExpression(UnOpType type, Variable* variable, std::string dest)
    : optype(type), variable(variable), dest(dest){

}
IncExpression::~IncExpression(){
    delete variable;
}

UnOpType & IncExpression::getOptype(){
    return optype;
}

Variable * IncExpression::getVariable(){
    return variable;
}

std::string IncExpression::getDest(){
    return dest;
}