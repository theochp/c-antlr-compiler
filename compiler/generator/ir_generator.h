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
#include "../ast/unexpression.h"
#include "../ast/return.h"
#include "../ast/variable.h"

using namespace std;

class IRGenerator {
    Node *ast;
    map<string, int> symbolTable;
    vector<Instruction*> instructions;
    int tempVarCount = 0;
    int stackOffset;


    const Instruction *generateBlock(const Block *block);
    const Instruction *generateStatement(const Statement *statement);
    const Instruction *generateConstant(const Constant *constant);
    const Instruction *generateDeclaration(const Declaration *declaration);
    const Instruction *generateExpression(const Expression *expression);
    const Instruction *generateUnExpression(const UnExpression *expression);
    const Instruction *generateReturn(const Return *ret);
    const Instruction *generateVariable(const Variable *variable);

    string newTempVar();

public:
    void generate();
    IRGenerator(Node *ast, map<string, int> symbolTable, int stackOffset);
    const map<string, int>& getSymbolTable();
    const vector<Instruction*>& getInstructions();
    void genConstant(Constant *constant);
};