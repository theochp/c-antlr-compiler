#pragma once

#include <iostream>

using namespace std;

class Error {
public:
    Error();
    virtual ~Error();
    virtual string getMessage()=0;
};


