#include <exception>
using namespace std;

#include "error.h"

class doubleDeclaration : public error {

public:
    doubleDeclaration(string nomVar, int lineNb=0, int charPosition=0);
    ~doubleDeclaration();
    string getMessage();
private:
    string nomVar;
    int lineNb;
    int charPosition;
};


