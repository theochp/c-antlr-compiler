#pragma once

#include <iostream>
#include <vector>

#include "irblock.h"

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
        call,
        equalcomp,
        diffcomp,
        infcomp,
        infeqcomp,
        supcomp,
        supeqcomp,
        bitwise_and,
        bitwise_or,
        bitwise_xor,
        bitwise_not,
        preincre,
        predecre,
        postincre,
        postdecre,
        logicalNot
    } IROp;

class IRBlock;

class Instruction {
public:
    Instruction(IROp op, const IRBlock *block);
    Instruction(IROp op, string dest, const IRBlock *block);
    Instruction(IROp op, string dest, const vector<string>& operands, const IRBlock *block);
    IROp op() const;
    string dest() const;
    const vector<string>& operands();
    const IRBlock *getBlock();

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
    const IRBlock *block;
};
