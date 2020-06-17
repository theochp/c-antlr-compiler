#include "return.h"

Return::Return() : statement(nullptr) {

}
    
Return::Return(Statement* statement) 
    : statement(statement) {

}

Return::~Return() {
    delete statement;
}

const Statement *Return::getStatement() const {
    return statement;
}

std::string Return::print() {
    std::string res("return(");
    res.append(statement->print());
    res.append(")");
    return res;
}