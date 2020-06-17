#include <assert.h>

#include "ir_generator.h"
#include "../ast/func.h"

IRGenerator::IRGenerator(Node *ast, map<string, int> symbolTable, int stackOffset)
    : ast(ast), symbolTable(symbolTable), stackOffset(stackOffset) {

}

void IRGenerator::generate() {
    if (const Func *func = dynamic_cast<const Func *>(ast)) {
        generateFunc(func);
    }
}

const IRBlock *IRGenerator::generateFunc(const Func *func) {
    IRBlock *block = new IRBlock(func->getName());
    generateBlock(func->getBlock(), block);
    blocks.push_back(block);
    return block;
}

const Instruction *IRGenerator::generateBlock(const Block *block, IRBlock *irBlock) {
    const Instruction *last;
    for (auto it = block->getStatements().begin(); it != block->getStatements().end(); ++it) {
        last = generateStatement(*it, irBlock);
    }
    return last;
}

const Instruction *IRGenerator::generateStatement(const Statement *statement, IRBlock *block) {
    if (const Constant *el = dynamic_cast<const Constant *>(statement)) {
        return generateConstant(el, block);
    }
    else if (const Declaration *el = dynamic_cast<const Declaration *>(statement)) {
        return generateDeclaration(el, block);
    }
    else if (const Expression *el = dynamic_cast<const Expression *>(statement)) {
        return generateExpression(el, block);
    }
    else if (const Return *el = dynamic_cast<const Return *>(statement)) {
        return generateReturn(el, block);
    }
    else if (const Variable *el = dynamic_cast<const Variable *>(statement)) {
        return generateVariable(el, block);
    } else if(const UnExpression *el = dynamic_cast<const UnExpression *>(statement)) {
        return generateUnExpression(el, block);
    } else if(const FuncCall *el = dynamic_cast<const FuncCall *>(statement)) { 
           return generateCall(el, block); 
    } else {
        assert("Need to handle new types");
    }
    return nullptr;
}

const Instruction *IRGenerator::generateConstant(const Constant *constant, IRBlock *block) {
    string dest = newTempVar();
    auto instr = new Instruction(IROp::ldcst, dest, {to_string(constant->getValue())});
    block->addInstruction(instr);
    return instr;
}

// TODO: changer la manière dont on gère les déclarations
const Instruction *IRGenerator::generateDeclaration(const Declaration *declaration, IRBlock *block) {
    string dest = newTempVar();
    Instruction *instr = nullptr;
    for (auto it = declaration->getSymbols().begin(); it != declaration->getSymbols().end(); ++it) {
        auto assignement = *it;
        string name = (*it).first;
        Statement *value = (*it).second;
        if (value != nullptr) {
            auto assignStmnt = generateStatement(value, block);
            string dest = newTempVar();
            instr = new Instruction(IROp::store, name, {assignStmnt->dest()});
            block->addInstruction(instr);
        }
    }

    return instr;
}

const Instruction *IRGenerator::generateExpression(const Expression *expression, IRBlock *block) {
    if (expression->getOp().type() == OpType::ASSIGN) {
        if(const Variable *dest = dynamic_cast<const Variable *>(expression->getLeft())) {
            string destName = dest->getName();
            auto rightInstr = generateStatement(expression->getRight(), block);
            auto instr = new Instruction(IROp::store, destName, {rightInstr->dest()});
            block->addInstruction(instr);
            return instr;
        } else {
            assert("la partie droite d'une affectation doit toujours être une variable. La grammaire ne doit pas permettre d'arriver ici");
            return nullptr;
        }
    } else {
        auto leftInstr = generateStatement(expression->getLeft(), block);
        auto rightInstr = generateStatement(expression->getRight(), block);
        string op1 = leftInstr->dest();
        string op2 = rightInstr->dest();
        string dest = newTempVar();
        
        Instruction *inst;
        switch(expression->getOp().type()) {
            case OpType::ADD:
                inst = new Instruction(IROp::add, dest, {op1, op2});
                break;
            case OpType::MINUS:
                inst = new Instruction(IROp::sub, dest, {op1, op2});
                break;
            case OpType::MULT:
                inst = new Instruction(IROp::mul, dest, {op1, op2});
                break;
            case OpType::DIV:
                inst = new Instruction(IROp::div, dest, {op1, op2});
            case OpType::ASSIGN:
                assert("Le cas ASSIGN doit être géré d'une autre manière");
                break;
            default:
                assert("Missing type");
                break;
        }

        block->addInstruction(inst);

        return inst;
    }
}

const Instruction *IRGenerator::generateUnExpression(const UnExpression *expression, IRBlock *block) {
    auto instr = generateStatement(expression->getExpr(), block);

    if (expression->getOp().type() == UnOpType::UN_PLUS) {
        return instr;
    }

    string op1 = instr->dest();
    string dest = newTempVar();
    
    Instruction *inst;
    switch(expression->getOp().type()) {
        case UnOpType::UN_PLUS:
            // nothing has to be done
            break;
        case UnOpType::UN_MINUS:
            inst = new Instruction(IROp::neg, dest, {op1});
            break;
        default:
            assert("Missing type");
            break;
    }

    block->addInstruction(inst);

    return inst;
}

const Instruction *IRGenerator::generateReturn(const Return *ret, IRBlock *block) {
    auto lastInstr = generateStatement(ret->getStatement(), block);
    auto instr = new Instruction(IROp::ret, string(""), {lastInstr->dest()});
    block->addInstruction(instr);
    return instr;
}

const Instruction *IRGenerator::generateVariable(const Variable *variable, IRBlock *block) {
    string dest = newTempVar();
    auto instr = new Instruction(IROp::store, dest, {variable->getName()});
    block->addInstruction(instr);
    return instr;
}

const Instruction *IRGenerator::generateCall(const FuncCall *func, IRBlock *block) {
    //TODO: handle func return
    string dest = newTempVar();
    vector<string> operands;
    operands.push_back(func->getName());
    for (auto it = func->getParamStatements().begin(); it != func->getParamStatements().end(); ++it) {
        auto statementInstr = generateStatement(*it, block);
        operands.push_back(statementInstr->dest());
    }
    auto instr = new Instruction(IROp::call, dest, operands);
    block->addInstruction(instr);
    return instr;
}

const map<string, int>& IRGenerator::getSymbolTable() {
    return symbolTable;
}

const vector<IRBlock*>& IRGenerator::getBlocks() {
    return blocks;
}

string IRGenerator::newTempVar() {
    int offset = stackOffset -= 4;
	string name("0_");
	name.append(to_string(1000 + tempVarCount++));
	symbolTable.emplace(name, offset);

	return name;
}