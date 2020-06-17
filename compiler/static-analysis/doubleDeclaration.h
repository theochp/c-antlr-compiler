#include <exception>
using namespace std;

#include "warning.h"

class doubleDeclaration : public warning {

public:
    doubleDeclaration(string nomVar, int lineNb=0);
    ~doubleDeclaration();
    string getMessage();
private:
    string nomVar;
    int lineNb;
};


