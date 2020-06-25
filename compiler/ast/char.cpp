#include "char.h"

Char::Char(char value) : value(value) {

}

char Char::getValue() const {

}

std::string Char::print() {
    std::string res("(");
    res.append(value);
    res.append(")");
    return res;
}