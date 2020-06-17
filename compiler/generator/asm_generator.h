#pragma once
#include <vector>
#include <map>
#include "../ir/instruction.h"
#include "../ast/node.h"

using namespace std;

class AsmGenerator {

    vector<instruction*> instructions;
    map<string, int> symbolTable;

    string generate_cst(instruction& inst);
    string generate_load(instruction& inst);
    string generate_store(instruction& inst);
    string generate_ret(instruction& inst);
    string generate_add(instruction& inst);
    string generate_sub(instruction& inst);
    string generate_mul(instruction& inst);
    string generate_neg(instruction& inst);

    string getOffsetRegister(string symbolName);

public:
    // constructor
    AsmGenerator(vector<instruction*> instructions, map<string, int> symbolTable);

    void generate(ostream& os);
};