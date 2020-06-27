#include <assert.h>

#include "ir_generator.h"
#include "../ast/func.h"

IRGenerator::IRGenerator(vector<const Node *> ast, map<string, map<string, int>> symbolTables, int stackOffset)
    : ast(ast), symbolTables(symbolTables), stackOffset(stackOffset) {

}

void IRGenerator::generate() {
    for (auto it = ast.begin(); it != ast.end(); ++it) {
        if (const Func *func = dynamic_cast<const Func *>(*it)) {
            generateFunc(func);
        }
    }
}

const IRFunc *IRGenerator::generateFunc(const Func *func) {
    IRFunc *irFunc = new IRFunc(func->getName());
    for (auto param : func->getParams()) {
        irFunc->addParam(param->getName());
    }
    generateBlock(func->getBlock(), irFunc);
    funcs.push_back(irFunc);
    return irFunc;
}

const IRBlock *IRGenerator::generateBlock(const Block *block, IRFunc *irFunc) {
    IRBlock *irBlock = new IRBlock(irFunc->getName(), irFunc);
    irFunc->addBlock(irBlock);
    for (auto it = block->getStatements().begin(); it != block->getStatements().end(); ++it) {
        generateStatement(*it, irBlock);
    }
    return irBlock;
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
    } 
    else if(const UnExpression *el = dynamic_cast<const UnExpression *>(statement)) {
        return generateUnExpression(el, block);
    } else if (const ArrayDeclaration *el = dynamic_cast<const ArrayDeclaration *>(statement)){
        return generateArray((ArrayDeclaration *)el , block);
    } else if(const ArrayValue *el = dynamic_cast<const ArrayValue *>(statement)){
        return generateArrayValue((ArrayValue*)el, block);
    } else if(const FuncCall *el = dynamic_cast<const FuncCall *>(statement)) { 
           return generateCall(el, block); 
    } else if (const LogicalNot *el = dynamic_cast<const LogicalNot *>(statement)){
        return generateLogicalNot(el, block);
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

// TODO: changer la manière dont on gère les déclarations
const Instruction *IRGenerator::generateDeclaration(const Declaration *declaration, IRBlock *block) {
    string dest = newTempVar(block->getFunc()->getName());

    Instruction *instr = nullptr;
    for (auto it = declaration->getSymbols().begin(); it != declaration->getSymbols().end(); ++it) {
        auto assignement = *it;
        string name = (*it).first;
        Statement *value = (*it).second.first;
        if (value != nullptr) {
            Instruction * instr;

            auto assignStmnt = generateStatement(value, block);
            if(assignStmnt != nullptr){
                string dest = newTempVar(block->getFunc()->getName());
                instr = new Instruction(IROp::store, name, {assignStmnt->dest()}, block);
            }
        
            block->addInstruction(instr);
        }
    }

    return instr;
}

const Instruction *IRGenerator::generateExpression(const Expression *expression, IRBlock *block) {
    if (expression->getOp().type() == OpType::ASSIGN) {
        if(const Variable *dest = dynamic_cast<const Variable *>(expression->getLeft())) {
            
            if(expression->getOffSet() != nullptr){
                string destName = dest->getName();
                auto offset = generateStatement(expression->getOffSet(), block);
                                
                auto rightInstr = generateStatement(expression->getRight(), block);
                auto instr = new Instruction(IROp::storeT, rightInstr->dest(), {dest->getName(), offset->dest()}, block);
                block->addInstruction(instr);
                return instr;

            }else{
                string destName = dest->getName();
                auto rightInstr = generateStatement(expression->getRight(), block);
                auto instr = new Instruction(IROp::store, destName, {rightInstr->dest()}, block);
                block->addInstruction(instr);
                return instr;
            } 
        } else {
            assert("la partie droite d'une affectation doit toujours être une variable. La grammaire ne doit pas permettre d'arriver ici");
            return nullptr;
        }
    } else {
        auto leftInstr = generateStatement(expression->getLeft(), block);
        auto rightInstr = generateStatement(expression->getRight(), block);
        string op1 = leftInstr->dest();
        string op2 = rightInstr->dest();
        string dest = newTempVar(block->getFunc()->getName());
        
        Instruction *inst;
        switch(expression->getOp().type()) {
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
                assert("Le cas ASSIGN doit être géré d'une autre manière");
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
    auto lastInstr = generateStatement(ret->getStatement(), block);
    auto instr = new Instruction(IROp::ret, string(""), {lastInstr->dest()}, block);
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
        auto statementInstr = generateStatement(*it, block);
        operands.push_back(statementInstr->dest());
    }
    auto instr = new Instruction(IROp::call, dest, operands, block);
    block->addInstruction(instr);
    return instr;
}

const Instruction *IRGenerator::generateLogicalNot(const LogicalNot *expr, IRBlock *block) {
    auto lastInstr = generateStatement(expr->getExpr(), block);
    string dest = newTempVar(block->getFunc()->getName());
    auto instr = new Instruction(IROp::logicalNot, dest, {lastInstr->dest()}, block);
    block->addInstruction(instr);
    return instr;
}

const Instruction *IRGenerator::generateArray(ArrayDeclaration *array, IRBlock *block) {
    for(int i = 0; i < array->Size() && i < array->Values().size(); i++)
    {
        auto stm = new Constant(atoi(array->Values().at(i).c_str()));
        auto instr = new Instruction(IROp::store, array->Names().at(i), {generateStatement(stm, block)->dest()}, block);
        block->addInstruction(instr);
    }
    
    return nullptr;
}

const Instruction *IRGenerator::generateArrayValue(ArrayValue *variable, IRBlock *block){
    auto expr = generateStatement(variable->getOffset(), block);
    string dest = newTempVar(block->getFunc()->getName());
    auto instr = new Instruction(IROp::loadT, dest, {variable->getArrayBegin().getName(), expr->dest()}, block);
    
    block->addInstruction(instr);
    
    return instr;
}

const map<string, map<string, int>>& IRGenerator::getSymbolTables() {
    return symbolTables;
}

const vector<IRFunc*>& IRGenerator::getFuncs() {
    return funcs;
}

string IRGenerator::newTempVar(string symbolTable) {
    int offset = stackOffset -= 4;
	string name("0_");
	name.append(to_string(1000 + tempVarCount++));
	symbolTables.at(symbolTable).emplace(name, offset);

	return name;
}