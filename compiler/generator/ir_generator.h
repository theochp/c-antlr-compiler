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
#include "../ast/char.h"
#include "../ast/declaration.h"
#include "../ast/expression.h"
#include "../ast/operator.h"
#include "../ast/unexpression.h"
#include "../ast/arrayDeclaration.h"
#include "../ast/arrayValue.h"
#include "../ast/logicalNot.h"
#include "../ast/return.h"
#include "../ast/variable.h"
#include "../ast/incexpression.h"
#include "../ast/func.h"
#include "../ast/funccall.h"
#include "../ast/ifelse.h"
#include "../ast/while.h"
#include "../ast/for.h"

using namespace std;

class IRGenerator {
    vector<const Node *> ast;
    map<string, map<string, int>> symbolTables;
    map<string, int> symbolOffsets;
    vector<IRFunc*> funcs;
    int tempVarCount = 0;
    int labelCount = 0;

    const IRFunc *generateFunc(const Func *func);
    pair<IRBlock *, IRBlock *>generateBlock(const Block *block, IRFunc *irFunc, string name);
    IRBlock *generateStatement(const Statement* statement, IRBlock *block);
    IRBlock *generateIfElse(const IfElse *ifElse, IRBlock *block);
    IRBlock *generateWhile(const While *aWhile, IRBlock *block);
    IRBlock *generateFor(const For *aFor, IRBlock *block);
    const Instruction *generateExpression(const Expression *statement, IRBlock *block);
    const Instruction *generateOperator(const Operator *pOperator, IRBlock *block);
    const Instruction *generateConstant(const Constant *constant, IRBlock *block);
    const Instruction *generateChar(const Char *character, IRBlock *block);
    const Instruction *generateDeclaration(const Declaration *declaration, IRBlock *block);
    const Instruction *generateUnExpression(const UnExpression *expression, IRBlock *block);
    const Instruction *generateLogicalNot(const LogicalNot *expression, IRBlock *block);
    const Instruction *generateReturn(const Return *ret, IRBlock *block);
    const Instruction *generateVariable(const Variable *variable, IRBlock *block);
    const Instruction *generateArray(ArrayDeclaration *variable, IRBlock *block);
    const Instruction *generateArrayValue(ArrayValue *variable, IRBlock *block);
    const Instruction *generateCall(const FuncCall *call, IRBlock *block);
    const Instruction *generateInc(IncExpression * expression, IRBlock *block);
    string newTempVar(const string& symbolTable);
    string newLabel();
    int incrementOffset(const string& func, int size) {
        int newOffset = symbolOffsets.at(func) -= size;
        symbolOffsets.emplace(func, newOffset);
        return newOffset;
    }
public:
    void generate();
    IRGenerator(vector<const Node *>, map<string, map<string, int>> symbolTables, map<string, int> symbolOffsets);
    const map<string, map<string, int>>& getSymbolTables();
    const vector<IRFunc*>& getFuncs();
};