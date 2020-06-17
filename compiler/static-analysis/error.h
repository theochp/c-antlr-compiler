#include <iostream>
#pragma once
using namespace std;

class error {
public:
    error();
    virtual ~error();
    virtual string getMessage()=0;
};


