#pragma once
#include <vector>
#include <map>
#include "../ir/instruction.h"
#include "../ir/irfunc.h"
#include "../ast/node.h"

using namespace std;

class AsmGenerator {

    vector<IRFunc*> funcs;
    map<string, map<string, int>> symbolTables;

    string generate_block(const IRBlock& block);

    string generate_ldcst(Instruction& inst);
    string generate_store(Instruction& inst);
    string generate_ret(Instruction& inst);
    string generate_add(Instruction& inst);
    string generate_sub(Instruction& inst);
    string generate_mul(Instruction& inst);
    string generate_div(Instruction& inst);
    string generate_neg(Instruction& inst);
    string generate_loadT(Instruction& inst);
    string generate_storeT(Instruction& inst);
    string generate_call(Instruction& inst);
    string generate_equal_comp(Instruction& inst);
    string generate_diff_comp(Instruction& inst);
    string generate_inf_comp(Instruction& inst);
    string generate_inf_eq_comp(Instruction& inst);
    string generate_sup_comp(Instruction& inst);
    string generate_sup_eq_comp(Instruction& inst);
    string generate_bitwise_and(Instruction& inst);
    string generate_bitwise_or(Instruction& inst);
    string generate_bitwise_xor(Instruction& inst);
    string generate_bitwise_not(Instruction& inst);
    string generate_not(Instruction& inst);
    string generate_je(Instruction& inst);
    string generate_jmp(Instruction& inst);
    string generate_preincre(Instruction& inst);
    string generate_postincre(Instruction& inst);
    string generate_predecre(Instruction& inst);
    string generate_postdecre(Instruction& inst);

    string getOffsetRegister(string symbolTable, string symbolName);
    string getOffset(string symbolTable, string symbolName);

public:
    // constructor
    AsmGenerator(vector<IRFunc*> funcs, map<string, map<string, int>> symbolTables);

    void generate(ostream& os);
};