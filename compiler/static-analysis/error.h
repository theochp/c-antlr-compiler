#pragma once

#include <iostream>

using namespace std;

class Error {
protected:
    string AS_COMMENT = "; ";
public:
    Error();
    virtual ~Error();
    virtual string getMessage()=0;
};


