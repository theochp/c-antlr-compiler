#include "warning.h"

class unusedVariable : public warning {
public:
    unusedVariable(string nomVar, int lineNb=0, int charPosition=0);
    ~unusedVariable();
    string getMessage();
private:
    string nomVar;
    int lineNb;
    int charPosition;
};
