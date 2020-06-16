#pragma once

#include <map>
#include <string>

#include "../ir/instruction.h"
#include "../ast/node.h"
#include "../ast/block.h"
#include "../ast/statement.h"
#include "../ast/constant.h"
#include "../ast/declaration.h"
#include "../ast/expression.h"
#include "../ast/return.h"
#include "../ast/variable.h"

using namespace std;

class IRGenerator {
    Node *ast;
    map<string, int> symbolTable;
    vector<instruction*> instructions;
    int tempVarCount = 0;
    int stackOffset;

    const instruction *generateBlock(const Block *block);
    const instruction *generateStatement(const Statement *statement);
    const instruction *generateConstant(const Constant *constant);
    const instruction *generateDeclaration(const Declaration *declaration);
    const instruction *generateExpression(const Expression *expression);
    const instruction *generateReturn(const Return *ret);
    const instruction *generateVariable(const Variable *variable);

    string newTempVar();

public:
    void generate();
    IRGenerator(Node *ast, map<string, int> symbolTable, int stackOffset);
    const map<string, int>& getSymbolTable();
    const vector<instruction*>& getInstructions();
    void genConstant(Constant *constant);
};