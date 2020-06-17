#include "error.h"

class undeclaredVariable : public error{

public:
    undeclaredVariable(string nomVar, int lineNb=0, int charPosition=0);
    ~undeclaredVariable();
    string getMessage();
private:
    string nomVar;
    int lineNb;
    int charPosition;
};
