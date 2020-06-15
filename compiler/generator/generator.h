#pragma once
#include <vector>
#include <map>
#include "../ir/instruction.h"
#include "../ast/node.h"

using namespace std;

class generator {

    Node *ast;
    vector<instruction*> instructions;
    map<string, int> symbolTable;

    string generate_cst(instruction& inst);
    string generate_load(instruction& inst);
    string generate_store(instruction& inst);
    string generate_ret(instruction& inst);

    string getOffsetRegister(string symbolName);

public:
    // constructor
    generator(Node* ast, map<string, int> symbolTable);

    void generate(ostream& os);
    string dfs(Node *node);
};