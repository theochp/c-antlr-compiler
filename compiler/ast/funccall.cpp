#include "funccall.h"

FuncCall::FuncCall(string name) : name(name) {

}

FuncCall::~FuncCall() {
    for( auto it = paramStatements.begin(); it != paramStatements.end(); ++it) {
        delete *it;
    }
}

string FuncCall::getName() const {
    return name;
}

const vector<Expression*>& FuncCall::getParamStatements() const {
    return paramStatements;
}

void FuncCall::addParamStatement(Expression *paramStatement) {
    if (paramStatement != nullptr) {
        paramStatements.push_back(paramStatement);
    }
}

void FuncCall::addParamStatements(const vector<Expression *>& statements) {
    copy(statements.begin(), statements.end(), back_inserter(paramStatements));
}
