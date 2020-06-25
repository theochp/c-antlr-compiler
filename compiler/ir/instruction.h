#pragma once

#include <iostream>
#include <vector>

using namespace std;

    typedef enum {
        ldcst,
        store,
        ret,
        add,
        sub,
        mul,
        div,
        neg,
        bitwise_and,
        bitwise_or,
        bitwise_xor,
        bitwise_not,
        logicalNot,
    } IROp;

class Instruction {
public:
    Instruction(IROp op);
    Instruction(IROp op, string dest);
    Instruction(IROp op, string dest, const vector<string>& operands);
    IROp op() const;
    string dest() const;
    const vector<string>& operands();

    /**
     * Allows to retrieve instructions operands.
     * Asserts if the operand list is long enough
     * 
     * @param i The opreand position (starting at 0)
     * @return The operand symbol name
     *
     */
    const string& operand(int i);

    friend std::ostream& operator<<(std::ostream& os, const Instruction& obj);
private: 
    IROp _op;
    string _dest;
    vector<string> _operands;
};
