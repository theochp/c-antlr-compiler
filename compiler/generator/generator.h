#pragma once
#include <vector>
#include <map>
#include "../ir/instruction.h"

using namespace std;

class generator {

    vector<instruction*> instructions;
    map<string, int> symbolTable;

    string generate_cst(instruction& inst);
    string generate_load(instruction& inst);
    string generate_store(instruction& inst);
    string generate_ret(instruction& inst);

    string getOffsetRegister(string symbolName);

public:
    // constructor
    generator(vector<instruction*> instructions, map<string, int> symbolTable);

    void generate(ostream& os);
};