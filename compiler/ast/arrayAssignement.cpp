#include "arrayAssignement.h"

ArrayAssignement::ArrayAssignement(){ }

ArrayAssignement::~ArrayAssignement(){ }

void ArrayAssignement::AddValue(std::string value){
    values.push_back(value);
}

std::size_t ArrayAssignement::Size(){
    return values.size();
}

std::string ArrayAssignement::print(){
    std::string res("{");
    for (auto it = values.begin(); it != values.end(); ++it) {
        res.append("\t");
        res.append((*it));
        res.append("\n");
    }
    res.append("}");
    return res;
}