#include "arrayDeclaration.h"
#include <iostream>
ArrayDeclaration::ArrayDeclaration(size_t size) 
    : size(size)
{ }
ArrayDeclaration::ArrayDeclaration() 
{ }

ArrayDeclaration::~ArrayDeclaration(){ 
    delete lvalue;
}

void ArrayDeclaration::AddExpression(Statement * expr){
    expressions.push_back(expr);
}

const std::size_t& ArrayDeclaration::Size(){
    return size;
}

void ArrayDeclaration::SetSize(size_t size){
    this->size = size;
}

std::string ArrayDeclaration::print(){
    std::string res("{");
    for (auto it = names.begin(); it != names.end(); ++it) {
        
        res.append("\t");
        res.append((*it));
        res.append("\n");
    }
    res.append("}");
    return res;
}

void ArrayDeclaration::AddNames(std::vector<std::string> names){
    for(auto it = names.begin(); it != names.end(); it++)
        this->names.push_back(*it);
}


void ArrayDeclaration::SetFirstName(std::string name){
    if(names.size() == 0){
        names.push_back(name);
    }
}

const std::vector<Statement *>& ArrayDeclaration::Expressions(){
    return expressions;
}

const std::vector<std::string>& ArrayDeclaration::Names(){
    return names;
}