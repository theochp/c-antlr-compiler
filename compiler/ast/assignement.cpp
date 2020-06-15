#include "assignement.h"

Assignement::Assignement(Variable *lvalue, Expression *rvalue) 
    : lvalue(lvalue), rvalue(rvalue) {

}
Assignement::~Assignement() {
    delete lvalue;
    delete rvalue;
}