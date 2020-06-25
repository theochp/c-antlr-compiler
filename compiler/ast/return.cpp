#include "return.h"

Return::Return() : statement(nullptr) {

}
    
Return::Return(Expression* statement)
    : statement(statement) {

}

Return::~Return() {
    delete statement;
}

const Expression *Return::getStatement() const {
    return statement;
}

std::string Return::print() {
    std::string res("return(");
    res.append(statement->print());
    res.append(")");
    return res;
}