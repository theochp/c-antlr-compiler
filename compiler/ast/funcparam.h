#pragma once

#include <string>

using namespace std;

class FuncParam {
    //Type type;
    string name;

public:
    explicit FuncParam(string name);
    string getName() const;
};