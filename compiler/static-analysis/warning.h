#include <iostream>

using namespace std;

class Warning {
public:
    Warning();
    virtual ~Warning();
    virtual string getMessage()=0;
};


