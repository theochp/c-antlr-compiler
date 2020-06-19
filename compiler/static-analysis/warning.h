#pragma once

#include <iostream>

using namespace std;

class Warning {
protected:
    string AS_COMMENT = "# ";
public:
    Warning();
    virtual ~Warning();
    virtual string getMessage()=0;
};


