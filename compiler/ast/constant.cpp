#include "constant.h"

Constant::Constant(int value) : value(value) {

}

std::string Constant::print() {
    std::string res("(");
    res.append(std::to_string(value));
    res.append(")");
    return res;
}
