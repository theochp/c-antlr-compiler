#include <cassert>
#include <utility>

#include "ir_generator.h"
#include "../ast/func.h"
#include "../ast/ifelse.h"

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
    if (auto el = dynamic_cast<const Expression *>(statement)) {
        generateExpression(el, block);
    }
    else if (auto el = dynamic_cast<const IfElse *>(statement)) {
        // return the continuation block
        return generateIfElse(el, block);
    } else if(auto el = dynamic_cast<const Declaration *>(statement)) {
        generateDeclaration(el, block);
    } else {
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
    else if (const Return *el = dynamic_cast<const Return *>(expression)) {
        return generateReturn(el, block);
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
                string destName = dest->getName();
                auto rightInstr = generateExpression(pOperator->getRight(), block);
                auto instr = new Instruction(IROp::store, destName, {rightInstr->dest()}, block);
                block->addInstruction(instr);
                return instr;
            } 
        } else {
            assert("la partie droite d'une affectation doit toujours être une variable. La grammaire ne doit pas permettre d'arriver ici");
            return nullptr;
        }
    } else {
        auto leftInstr = generateExpression(pOperator->getLeft(), block);
        auto rightInstr = generateExpression(pOperator->getRight(), block);
        string op1 = leftInstr->dest();
        string op2 = rightInstr->dest();
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
    auto lastInstr = generateExpression(ret->getStatement(), block);
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
