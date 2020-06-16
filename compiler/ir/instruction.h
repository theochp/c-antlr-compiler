#pragma once
#include <iostream>
using namespace std;

typedef enum {
    cst,
    store,
    load,
    ret,
    add,
    sub,
    mul,
    div,
} inst_type;

class instruction {
    private: 
        inst_type _op;
        string _source = ""; // can be a constant or a symbol name
        string _dest = "";
        string _operand = "";

    public:
        instruction(inst_type opType);
        instruction(inst_type opType, string source);
        instruction(inst_type opType, string source, string dest);
        instruction(inst_type opType, string source, string dest, string operand);
        inst_type op() const;
        string source() const;
        string dest() const;
        string operand() const;

        friend std::ostream& operator<<(std::ostream& os, const instruction& obj);
};
