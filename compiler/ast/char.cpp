#include "char.h"

Char::Char(int value) : value(value) {

}

int Char::getValue() const {
    return value;
}

std::string Char::print() {
    std::string res("(");
    res.append(std::to_string(value));
    res.append(")");
    return res;
}