#pragma once
#include <iostream>
using namespace std;

typedef enum {
    cst,
    store,
    load,
    ret,
} inst_type;

class instruction {
    private: 
        inst_type _op;
        string _source; // can be a constant or a symbol name
        string _dest;
        string _operand;

    public:
        instruction(inst_type opType);
        instruction(inst_type opType, string source);
        instruction(inst_type opType, string source, string dest);
        instruction(inst_type opType, string source, string dest, string operand);
        inst_type op();
        string source();
        string dest();
        string operand();

        friend std::ostream& operator<<(std::ostream& os, const instruction& obj);
};
