#include "return.h"

Return::Return() : statement(nullptr) {

}
    
Return::Return(Statement* statement) 
    : statement(statement) {

}

std::string Return::print() {
    std::string res("return(");
    res.append(statement->print());
    res.append(")");
    return res;
}