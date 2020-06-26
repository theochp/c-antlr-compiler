#include "arrayDeclaration.h"
#include <iostream>
ArrayDeclaration::ArrayDeclaration(size_t size) 
    : size(size)
{ }
ArrayDeclaration::ArrayDeclaration() 
{ }

ArrayDeclaration::~ArrayDeclaration(){ }

void ArrayDeclaration::AddValue(std::string value){
    values.push_back(value);
}

const std::size_t& ArrayDeclaration::Size(){
    return size;
}

void ArrayDeclaration::SetSize(size_t size){
    this->size = size;
}

std::string ArrayDeclaration::print(){
    std::string res("{");
res.append("name size");
    res.append(std::to_string(names.size()));
    for (auto it = values.begin(); it != values.end(); ++it) {
        
        res.append("\t");
        res.append((*it));
        res.append("\n");
    }
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

const std::vector<std::string>& ArrayDeclaration::Values(){
    return values;
}

const std::vector<std::string>& ArrayDeclaration::Names(){
    return names;
}