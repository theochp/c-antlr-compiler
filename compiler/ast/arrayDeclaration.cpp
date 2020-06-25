#include "arrayDeclaration.h"

ArrayDeclaration::ArrayDeclaration(){ }

ArrayDeclaration::~ArrayDeclaration(){ }

void ArrayDeclaration::AddValue(std::string value){
    values.push_back(value);
}

std::size_t ArrayDeclaration::Size(){
    return values.size();
}

std::string ArrayDeclaration::print(){
    std::string res("{");
    for (auto it = values.begin(); it != values.end(); ++it) {
        res.append("\t");
        res.append((*it));
        res.append("\n");
    }
    res.append("}");
    return res;
}

void ArrayDeclaration::SetNames(std::vector<std::string> names){
    this->names = names;
}

const std::vector<std::string>& ArrayDeclaration::Values(){
    return values;
}

const std::vector<std::string>& ArrayDeclaration::Names(){
    return names;
}