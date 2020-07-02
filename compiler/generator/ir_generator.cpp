#include <cassert>
#include <utility>

#include "ir_generator.h"

IRGenerator::IRGenerator(vector<const Node *> ast, map<string, map<string, int>> symbolTables, map<string, int> symbolOffsets)
    : ast(std::move(ast)), symbolTables(std::move(symbolTables)), symbolOffsets(std::move(symbolOffsets)) {

}

void IRGenerator::generate() {
    for (auto & it : ast) {
        if (const Func *func = dynamic_cast<const Func *>(it)) {
            generateFunc(func);
        }
    }
}

const IRFunc *IRGenerator::generateFunc(const Func *func) {
    auto irFunc = new IRFunc(func->getName());
    for (auto param : func->getParams()) {
        irFunc->addParam(param->getName());
    }
    generateBlock(func->getBlock(), irFunc, irFunc->getName());
    symbolTables.at(func->getName()).emplace("!funcRet", incrementOffset(func->getName(), 4));
    funcs.push_back(irFunc);
    return irFunc;
}

pair<IRBlock *, IRBlock *> IRGenerator::generateBlock(const Block *block, IRFunc *irFunc, string name) {
    auto *irBlock = new IRBlock(name, irFunc);
    IRBlock *lastBlock = irBlock;
    irFunc->addBlock(irBlock);
    for (auto it : block->getStatements()) {
        lastBlock = generateStatement(it, lastBlock);
    }
    return {irBlock, lastBlock};
}

IRBlock *IRGenerator::generateStatement(const Statement* statement, IRBlock *block) {
    if (auto expr = dynamic_cast<const Expression *>(statement)) {
        generateExpression(expr, block);
    }
    else if (auto ifElse = dynamic_cast<const IfElse *>(statement)) {
        return generateIfElse(ifElse, block);
    }
    else if (auto aWhile = dynamic_cast<const While *>(statement)) {
        return generateWhile(aWhile, block);
    }
    else if (auto aFor = dynamic_cast<const For *>(statement)) {
        return generateFor(aFor, block);
    }
    else if(auto decl = dynamic_cast<const Declaration *>(statement)) {
        generateDeclaration(decl, block);
    }
    else if(auto ret = dynamic_cast<const Return *>(statement)) {
        generateReturn(ret, block);
    }
    else {
        assert("Need to handle new types");
    }
    return block;
}

IRBlock *IRGenerator::generateIfElse(const IfElse *ifElse, IRBlock *block) {
    // set condition var
    auto conditionLastIntr = generateExpression(ifElse->getCondition(), block);
    block->setTestVarName(conditionLastIntr->dest());

    // create the continuation block (after the if/else)
    auto continueBlock  = new IRBlock(newLabel(), block->getFunc());

    // create the if block
    auto blockIf = generateBlock(ifElse->getIfBlock(), block->getFunc(), newLabel());
    
    // the if block exits to the continuation block
    blockIf.second->setExitTrue(continueBlock);

    // the block before the if/else exists to the if block if the condition succeed
    block->setExitTrue(blockIf.first);

    if (ifElse->getElseBlock() != nullptr) {
        // create else block
        auto blockElse = generateBlock(ifElse->getElseBlock(), block->getFunc(), newLabel());

        // the else block exits to the continuation block
        blockElse.second->setExitTrue(continueBlock);

        // the block before the if/else exits to the else block if the condition fails
        block->setExitFalse(blockElse.first);
    } else {
        // the block before the if exits to the continuation block if the condition fails
        block->setExitFalse(continueBlock);
    }

    block->getFunc()->addBlock(continueBlock);

    // generate instructions
    block->addInstruction(new Instruction(je, block->getExitFalse()->getLabel(), {conditionLastIntr->dest(), "0"}, block));
    blockIf.second->addInstruction(new Instruction(jmp, {continueBlock->getLabel()}, blockIf.second));

    return continueBlock;
}

IRBlock *IRGenerator::generateWhile(const While *aWhile, IRBlock *block) {
    // set condition var
    auto conditionBlock = new IRBlock(newLabel(), block->getFunc());
    auto conditionLastIntr = generateExpression(aWhile->getCondition(), conditionBlock);
    block->setTestVarName(conditionLastIntr->dest());

    block->getFunc()->addBlock(conditionBlock);

    // create the while block
    auto blockWhile = generateBlock(aWhile->getBlock(), block->getFunc(), newLabel());

    // create the continuation block (after the while)
    auto continueBlock = new IRBlock(newLabel(), block->getFunc());

    conditionBlock->setExitTrue(blockWhile.first);
    conditionBlock->setExitFalse(continueBlock);
    blockWhile.second->setExitTrue(conditionBlock);

    // generate instructions
    conditionBlock->addInstruction(new Instruction(je, continueBlock->getLabel(), {conditionLastIntr->dest(), "0"}, block));
    blockWhile.second->addInstruction(new Instruction(jmp, {conditionBlock->getLabel()}, blockWhile.second));

    block->getFunc()->addBlock(continueBlock);

    return continueBlock;
}

IRBlock *IRGenerator::generateFor(const For *aFor, IRBlock *block) {

    // 1st for argument
    generateExpression(aFor->getExpressions()[0], block);

    // 2nd for argument
    auto conditionBlock = new IRBlock(newLabel(), block->getFunc());
    auto conditionLastIntr = generateExpression(aFor->getExpressions()[1], conditionBlock);
    block->setTestVarName(conditionLastIntr->dest());

    block->getFunc()->addBlock(conditionBlock);

    // create the for block
    auto blockFor = generateBlock(aFor->getBlock(), block->getFunc(), newLabel());

    // 3rd for argument
    generateExpression(aFor->getExpressions()[2], blockFor.second);

    auto continueBlock = new IRBlock(newLabel(), block->getFunc());

    conditionBlock->setExitTrue(blockFor.first);
    conditionBlock->setExitFalse(continueBlock);
    blockFor.second->setExitTrue(conditionBlock);

    // generate instructions
    conditionBlock->addInstruction(new Instruction(je, continueBlock->getLabel(), {conditionLastIntr->dest(), "0"}, block));
    blockFor.second->addInstruction(new Instruction(jmp, {conditionBlock->getLabel()}, blockFor.second));

    block->getFunc()->addBlock(continueBlock);

    return continueBlock;
}

const Instruction *IRGenerator::generateExpression(const Expression *expression, IRBlock *block) {
    if (const Constant *el = dynamic_cast<const Constant *>(expression)) {
        return generateConstant(el, block);
    }
    else if (const Char *el = dynamic_cast<const Char *>(expression)) {
        return generateChar(el, block);
    }
    else if (const Declaration *el = dynamic_cast<const Declaration *>(expression)) {
        return generateDeclaration(el, block);
    }
    else if (const Declaration *el = dynamic_cast<const Declaration *>(expression)) {
        return generateDeclaration(el, block);
    }
    else if (const Operator *el = dynamic_cast<const Operator *>(expression)) {
        return generateOperator(el, block);
    }
    else if (const Variable *el = dynamic_cast<const Variable *>(expression)) {
        return generateVariable(el, block);
    } 
    else if(const UnExpression *el = dynamic_cast<const UnExpression *>(expression)) {
        return generateUnExpression(el, block);
    } else if (const ArrayDeclaration *el = dynamic_cast<const ArrayDeclaration *>(expression)){
        return generateArray((ArrayDeclaration *)el , block);
    } else if(const ArrayValue *el = dynamic_cast<const ArrayValue *>(expression)){
        return generateArrayValue((ArrayValue*)el, block);
    } else if(const FuncCall *el = dynamic_cast<const FuncCall *>(expression)) { 
           return generateCall(el, block); 
    } else if (const LogicalNot *el = dynamic_cast<const LogicalNot *>(expression)){
        return generateLogicalNot(el, block);
    }else if(auto el = dynamic_cast<const IncExpression *>(expression)) {
        return generateInc((IncExpression*)expression, block);
    }
    else {
        assert("Need to handle new types");
    }
    return nullptr;
}

const Instruction *IRGenerator::generateConstant(const Constant *constant, IRBlock *block) {
    string dest = newTempVar(block->getFunc()->getName());
    auto instr = new Instruction(IROp::ldcst, dest, {to_string(constant->getValue())}, block);
    block->addInstruction(instr);
    return instr;
}

const Instruction *IRGenerator::generateChar(const Char *character, IRBlock *block) {
    string dest = newTempVar(block->getFunc()->getName());
    auto instr = new Instruction(IROp::ldcst, dest, {to_string(character->getValue())}, block);
    block->addInstruction(instr);
    return instr;
}

const Instruction *IRGenerator::generateOperator(const Operator *pOperator, IRBlock *block) {
    if (pOperator->getOp() == OpType::ASSIGN) {
        if(const Variable *dest = dynamic_cast<const Variable *>(pOperator->getLeft())) {
            if(pOperator->getOffset() != nullptr){
                string destName = dest->getName();
                auto offset = generateExpression(pOperator->getOffset(), block);
                auto rightInstr = generateExpression(pOperator->getRight(), block);
                auto instr = new Instruction(IROp::storeT, rightInstr->dest(), {dest->getName(), offset->dest()}, block);
                block->addInstruction(instr);
                return instr;
            } else {
                const string& destName = dest->getName();
                Instruction *instr;
                if (auto cst = dynamic_cast<const Constant *>(pOperator->getRight())) {
                    instr = new Instruction(IROp::ldcst, destName, {to_string(cst->getValue())}, block);
                } else {
                    auto rightInstr = generateExpression(pOperator->getRight(), block);
                    instr = new Instruction(IROp::store, destName, {rightInstr->dest()}, block);
                }

                block->addInstruction(instr);
                return instr;
            } 
        } else {
            static_assert("la partie droite d'une affectation doit toujours être une variable. La grammaire ne doit pas permettre d'arriver ici", "");
            return nullptr;
        }
    } else {
        string op1, op2;

        auto leftVar = dynamic_cast<const Variable*>(pOperator->getLeft());
        auto rightVar = dynamic_cast<const Variable*>(pOperator->getRight());

        if (leftVar) {
            op1 = leftVar->getName();
        } else {
            op1 = generateExpression(pOperator->getLeft(), block)->dest();
        }

        if (rightVar) {
            op2 = rightVar->getName();
        } else {
            op2 = generateExpression(pOperator->getRight(), block)->dest();
        }

        string dest = newTempVar(block->getFunc()->getName());
        
        Instruction *inst;
        switch(pOperator->getOp()) {
            case OpType::ADD:
                inst = new Instruction(IROp::add, dest, {op1, op2}, block);
                break;
            case OpType::MINUS:
                inst = new Instruction(IROp::sub, dest, {op1, op2}, block);
                break;
            case OpType::MULT:
                inst = new Instruction(IROp::mul, dest, {op1, op2}, block);
                break;
            case OpType::DIV:
                inst = new Instruction(IROp::div, dest, {op1, op2}, block);
                break;
            case OpType::EQUALCOMP:
                inst =  new Instruction(IROp::equalcomp, dest, {op1, op2}, block);
                break;
            case OpType::DIFFCOMP:
                inst =  new Instruction(IROp::diffcomp, dest, {op1, op2}, block);
                break;
            case OpType::INFCOMP:
                inst =  new Instruction(IROp::infcomp, dest, {op1, op2}, block);
                break;
            case OpType::INFEQCOMP:
                inst =  new Instruction(IROp::infeqcomp, dest, {op1, op2}, block);
                break;
            case OpType::SUPCOMP:
                inst =  new Instruction(IROp::supcomp, dest, {op1, op2}, block);
                break;
            case OpType::SUPEQCOMP:
                inst =  new Instruction(IROp::supeqcomp, dest, {op1, op2}, block);
                break;
            case OpType::ASSIGN:
                static_assert("Le cas ASSIGN doit être géré d'une autre manière", "");
                break;
            case OpType::BITWISE_AND:
                inst = new Instruction(IROp::bitwise_and, dest, {op1, op2}, block);
                break;
            case OpType::BITWISE_OR:
                inst = new Instruction(IROp::bitwise_or, dest, {op1, op2}, block);
                break;
            case OpType::BITWISE_XOR:
                inst = new Instruction(IROp::bitwise_xor, dest, {op1, op2}, block);
                break;
            default:
                static_assert("Missing type", "");
                break;
        }

        block->addInstruction(inst);

        return inst;
    }
}

// TODO: changer la manière dont on gère les déclarations
const Instruction *IRGenerator::generateDeclaration(const Declaration *declaration, IRBlock *block) {
    string dest = newTempVar(block->getFunc()->getName());
    Instruction *instr = nullptr;
    for (auto it = declaration->getSymbols().begin(); it != declaration->getSymbols().end(); ++it) {
        auto assignement = *it;
        string name = (*it).first;
        Expression *value = (*it).second.first;
        if (value != nullptr) {
            auto assignStmnt = generateExpression(value, block);
            if(assignStmnt != nullptr){
                string dest = newTempVar(block->getFunc()->getName());
                instr = new Instruction(IROp::store, name, {assignStmnt->dest()}, block);
                block->addInstruction(instr);
            }
        }
    }

    return instr;
}

const Instruction *IRGenerator::generateUnExpression(const UnExpression *expression, IRBlock *block) {
    auto instr = generateExpression(expression->getExpr(), block);

    if (expression->getOp().type() == UnOpType::UN_PLUS) {
        return instr;
    }

    string op1 = instr->dest();
    string dest = newTempVar(block->getFunc()->getName());

    Instruction *inst;
    switch(expression->getOp().type()) {
        case UnOpType::UN_PLUS:
            // nothing has to be done
            break;
        case UnOpType::UN_MINUS:
            inst = new Instruction(IROp::neg, dest, {op1}, block);
            break;
        case UnOpType::BITWISE_NOT:
            inst = new Instruction(IROp::bitwise_not, dest, {op1}, block);
            break;
        default:
            assert("Missing type");
            break;
    }

    block->addInstruction(inst);

    return inst;
}

const Instruction *IRGenerator::generateReturn(const Return *ret, IRBlock *block) {
    if (ret->getExpression() != nullptr) {
        auto lastInstr = generateExpression(ret->getExpression(), block);
        auto instr = new Instruction(store, "!funcRet", {lastInstr->dest()}, block);
        block->addInstruction(instr);
    } else {
        // return 0 by default
        auto instr = new Instruction(ldcst, "!funcRet", {"0"}, block);
        block->addInstruction(instr);
    }
    auto instr = new Instruction(IROp::jmp, ".RET" + block->getFunc()->getName(), block);
    block->addInstruction(instr);
    return instr;
}

const Instruction *IRGenerator::generateVariable(const Variable *variable, IRBlock *block) {
    string dest = newTempVar(block->getFunc()->getName());
    auto instr = new Instruction(IROp::store, dest, {variable->getName()}, block);
    block->addInstruction(instr);
    return instr;
}

const Instruction *IRGenerator::generateCall(const FuncCall *func, IRBlock *block) {
    //TODO: handle func return
    string dest = newTempVar(block->getFunc()->getName());
    vector<string> operands;
    operands.push_back(func->getName());
    for (auto it = func->getParamStatements().begin(); it != func->getParamStatements().end(); ++it) {
        auto statementInstr = generateExpression(*it, block);
        operands.push_back(statementInstr->dest());
    }
    auto instr = new Instruction(IROp::call, dest, operands, block);
    block->addInstruction(instr);
    return instr;
}

const Instruction *IRGenerator::generateLogicalNot(const LogicalNot *expr, IRBlock *block) {
    auto lastInstr = generateExpression(expr->getExpr(), block);
    string dest = newTempVar(block->getFunc()->getName());
    auto instr = new Instruction(IROp::logicalNot, dest, {lastInstr->dest()}, block);
    block->addInstruction(instr);
    return instr;
}

const Instruction *IRGenerator::generateArray(ArrayDeclaration *array, IRBlock *block) {
    for(int i = 0; i < array->Size() && i < array->Expressions().size(); i++) {
        auto instr = new Instruction(IROp::store, array->Names().at(i), {generateExpression(array->Expressions().at(i), block)->dest()}, block);
        block->addInstruction(instr);
    }
    
    return nullptr;
}

const Instruction *IRGenerator::generateArrayValue(ArrayValue *variable, IRBlock *block){
    auto expr = generateExpression(variable->getOffset(), block);
    string dest = newTempVar(block->getFunc()->getName());
    auto instr = new Instruction(IROp::loadT, dest, {variable->getArrayBegin().getName(), expr->dest()}, block);
    
    block->addInstruction(instr);
    
    return instr;
}

const Instruction *IRGenerator::generateInc(IncExpression * expression, IRBlock *block){
    string var = expression->getVariable()->getName();
    string dest = expression->getDest();
    
    Instruction *inst;
    switch(expression->getOptype()) {
        case UnOpType::POSTINCRE:
            inst = new Instruction(IROp::postincre, dest, {var}, block);
            break;
        case UnOpType::POSTDECRE:
            inst = new Instruction(IROp::postdecre, dest, {var}, block);
            break;
        case UnOpType::PREINCRE:
            inst = new Instruction(IROp::preincre, dest, {var}, block);
            break;
        case UnOpType::PREDECRE:
            inst = new Instruction(IROp::predecre, dest, {var}, block);
            break;
        default:
            assert("Missing type");
            break;
    }

    block->addInstruction(inst);

    return inst;
}

const map<string, map<string, int>>& IRGenerator::getSymbolTables() {
    return symbolTables;
}

const vector<IRFunc*>& IRGenerator::getFuncs() {
    return funcs;
}

string IRGenerator::newTempVar(const string& symbolTable) {
    int offset = incrementOffset(symbolTable, 4);
	string name("0_");
	name.append(to_string(1000 + tempVarCount++));
	symbolTables.at(symbolTable).emplace(name, offset);

	return name;
}

string IRGenerator::newLabel() {
    int count = labelCount++;
    string label = ".L";
    label += to_string(count);
    return label;
}
