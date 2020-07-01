#pragma once

#include <iostream>

using namespace std;

class ASMGenerator {
public:
    virtual void generate(ostream& os) = 0;
};
