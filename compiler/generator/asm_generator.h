#pragma once
#include <vector>
#include <map>
#include "../ir/instruction.h"
#include "../ir/irblock.h"
#include "../ast/node.h"

using namespace std;

class AsmGenerator {

    vector<IRBlock*> blocks;
    map<string, int> symbolTable;

    string generate_block(IRBlock& block);

    string generate_ldcst(Instruction& inst);
    string generate_store(Instruction& inst);
    string generate_ret(Instruction& inst);
    string generate_add(Instruction& inst);
    string generate_sub(Instruction& inst);
    string generate_mul(Instruction& inst);
    string generate_div(Instruction& inst);
    string generate_neg(Instruction& inst);
    string generate_equal_comp(Instruction& inst);
    string generate_diff_comp(Instruction& inst);
    string generate_inf_comp(Instruction& inst);
    string generate_inf_eq_comp(Instruction& inst);
    string generate_sup_comp(Instruction& inst);
    string generate_sup_eq_comp(Instruction& inst);
    string generate_bitwise_and(Instruction& inst);
    string generate_bitwise_or(Instruction& inst);
    string generate_bitwise_xor(Instruction& inst);
    string generate_not(Instruction& inst);

    string getOffsetRegister(string symbolName);

public:
    // constructor
    AsmGenerator(vector<IRBlock*> blocks, map<string, int> symbolTable);

    void generate(ostream& os);
};