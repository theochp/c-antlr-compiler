#include "funcparam.h"

FuncParam::FuncParam(string name) : name(name) {

}

string FuncParam::getName() const {
    return name;
}