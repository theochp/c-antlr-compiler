#pragma once

#include <map>
#include <string>

#include "../ir/instruction.h"
#include "../ir/irblock.h"
#include "../ast/node.h"
#include "../ast/block.h"
#include "../ast/statement.h"
#include "../ast/constant.h"
#include "../ast/char.h"
#include "../ast/declaration.h"
#include "../ast/expression.h"
#include "../ast/unexpression.h"
#include "../ast/return.h"
#include "../ast/variable.h"
#include "../ast/func.h"

using namespace std;

class IRGenerator {
    Node *ast;
    map<string, int> symbolTable;
    vector<IRBlock*> blocks;
    int tempVarCount = 0;
    int stackOffset;

    const IRBlock *generateFunc(const Func *func);
    const Instruction *generateBlock(const Block *block, IRBlock *irBlock);
    const Instruction *generateStatement(const Statement *statement, IRBlock *block);
    const Instruction *generateConstant(const Constant *constant, IRBlock *block);
    const Instruction *generateChar(const Char *character, IRBlock *block);
    const Instruction *generateDeclaration(const Declaration *declaration, IRBlock *block);
    const Instruction *generateExpression(const Expression *expression, IRBlock *block);
    const Instruction *generateUnExpression(const UnExpression *expression, IRBlock *block);
    const Instruction *generateReturn(const Return *ret, IRBlock *block);
    const Instruction *generateVariable(const Variable *variable, IRBlock *block);

    string newTempVar();

public:
    void generate();
    IRGenerator(Node *ast, map<string, int> symbolTable, int stackOffset);
    const map<string, int>& getSymbolTable();
    const vector<IRBlock*>& getBlocks();
    void genConstant(Constant *constant);
};