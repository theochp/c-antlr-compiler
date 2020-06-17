#pragma once
#include <vector>
#include <map>
#include "../ir/instruction.h"
#include "../ast/node.h"

using namespace std;

class AsmGenerator {

    vector<Instruction*> instructions;
    map<string, int> symbolTable;

    string generate_ldcst(Instruction& inst);
    string generate_store(Instruction& inst);
    string generate_ret(Instruction& inst);
    string generate_add(Instruction& inst);
    string generate_sub(Instruction& inst);
    string generate_mul(Instruction& inst);
    string generate_div(Instruction& inst);
    string generate_neg(Instruction& inst);

    string getOffsetRegister(string symbolName);

public:
    // constructor
    AsmGenerator(vector<Instruction*> instructions, map<string, int> symbolTable);

    void generate(ostream& os);
};