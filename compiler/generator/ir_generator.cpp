#include <cassert>
#include <utility>

#include "ir_generator.h"
#include "../ast/func.h"
#include "../ast/ifelse.h"

IRGenerator::IRGenerator(vector<const Node *> ast, map<string, map<string, int>> symbolTables, int stackOffset)
    : ast(std::move(ast)), symbolTables(std::move(symbolTables)), stackOffset(stackOffset) {

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

IRBlock *IRGenerator::generateBlock(const Block *block, IRFunc *irFunc, string name) {
    auto *irBlock = new IRBlock(name, irFunc);
    irFunc->addBlock(irBlock);
    for (auto it : block->getStatements()) {
        irBlock = generateStatement(it, irBlock);
    }
    return irBlock;
}

IRBlock *IRGenerator::generateStatement(const Statement* statement, IRBlock *block) {
    if (auto el = dynamic_cast<const Expression *>(statement)) {
        generateExpression(el, block);
    }
    else if (auto el = dynamic_cast<const IfElse *>(statement)) {
        // return the continuation block
        return generateIfElse(el, block);
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
    blockIf->setExitTrue(continueBlock);

    // the block before the if/else exists to the if block if the condition succeed
    block->setExitTrue(blockIf);

    if (ifElse->getElseBlock() != nullptr) {
        // create else block
        auto blockElse = generateBlock(ifElse->getElseBlock(), block->getFunc(), newLabel());

        // the else block exits to the continuation block
        blockElse->setExitTrue(continueBlock);

        // the block before the if/else exits to the else block if the condition fails
        block->setExitFalse(blockElse);
    } else {
        // the block before the if exits to the continuation block if the condition fails
        block->setExitFalse(continueBlock);
    }

    block->getFunc()->addBlock(continueBlock);

    // generate instructions
    block->addInstruction(new Instruction(je, block->getExitFalse()->getLabel(), {conditionLastIntr->dest(), "0"}, block));
    blockIf->addInstruction(new Instruction(jmp, {continueBlock->getLabel()}, blockIf));

    return continueBlock;
}

const Instruction *IRGenerator::generateExpression(const Expression *expression, IRBlock *block) {
    if (const Constant *el = dynamic_cast<const Constant *>(expression)) {
        return generateConstant(el, block);
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

// TODO: changer la manière dont on gère les déclarations
const Instruction *IRGenerator::generateDeclaration(const Declaration *declaration, IRBlock *block) {
    string dest = newTempVar(block->getFunc()->getName());
    Instruction *instr = nullptr;
    for (auto it = declaration->getSymbols().begin(); it != declaration->getSymbols().end(); ++it) {
        auto assignement = *it;
        string name = (*it).first;
        Expression *value = (*it).second;
        if (value != nullptr) {
            auto assignStmnt = generateExpression(value, block);
            string dest = newTempVar(block->getFunc()->getName());
            instr = new Instruction(IROp::store, name, {assignStmnt->dest()}, block);
            block->addInstruction(instr);
        }
    }

    return instr;
}

const Instruction *IRGenerator::generateOperator(const Operator *pOperator, IRBlock *block) {
    if (pOperator->getOp() == OpType::ASSIGN) {
        if(const Variable *dest = dynamic_cast<const Variable *>(pOperator->getLeft())) {
            string destName = dest->getName();
            auto rightInstr = generateExpression(pOperator->getRight(), block);
            auto instr = new Instruction(IROp::store, destName, {rightInstr->dest()}, block);
            block->addInstruction(instr);
            return instr;
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

string IRGenerator::newLabel() {
    int count = labelCount++;
    string label = ".L";
    label += to_string(count);
    return label;
}
