#include "variable.h"

Variable::Variable(string name) : name(name) {

}

const string& Variable::getName() const {
    return name;
}

std::string Variable::print() {
    std::string res("(");
    res.append(name);
    res.append(")");
    return res;
}