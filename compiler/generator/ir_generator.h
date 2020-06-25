#pragma once

#include <map>
#include <string>

#include "../ir/irfunc.h"
#include "../ir/irblock.h"
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
#include "../ast/func.h"
#include "../ast/funccall.h"

using namespace std;

class IRGenerator {
    vector<const Node *> ast;
    map<string, map<string, int>> symbolTables;
    vector<IRFunc*> funcs;
    int tempVarCount = 0;
    int stackOffset;

    const IRFunc *generateFunc(const Func *func);
    const IRBlock *generateBlock(const Block *block, IRFunc *irFunc);
    const Instruction *generateStatement(const Statement *statement, IRBlock *block);
    const Instruction *generateConstant(const Constant *constant, IRBlock *block);
    const Instruction *generateDeclaration(const Declaration *declaration, IRBlock *block);
    const Instruction *generateExpression(const Expression *expression, IRBlock *block);
    const Instruction *generateUnExpression(const UnExpression *expression, IRBlock *block);
    const Instruction *generateReturn(const Return *ret, IRBlock *block);
    const Instruction *generateVariable(const Variable *variable, IRBlock *block);
    const Instruction *generateCall(const FuncCall *call, IRBlock *block);
    string newTempVar(string symbolTable);

public:
    void generate();
    IRGenerator(vector<const Node *>, map<string, map<string, int>> symbolTables, int stackOffset);
    const map<string, map<string, int>>& getSymbolTables();
    const vector<IRFunc*>& getFuncs();
    void genConstant(Constant *constant);
};