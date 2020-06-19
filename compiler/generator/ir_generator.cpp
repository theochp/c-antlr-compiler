#include <assert.h>

#include "ir_generator.h"

IRGenerator::IRGenerator(Node *ast, map<string, int> symbolTable, int stackOffset)
    : ast(ast), symbolTable(symbolTable), stackOffset() {

}

void IRGenerator::generate() {
    if (const Block *block = dynamic_cast<const Block *>(ast)) {
        generateBlock(block);
    }
}

const Instruction *IRGenerator::generateBlock(const Block *block) {
    const Instruction *last;
    for (auto it = block->getStatements().begin(); it != block->getStatements().end(); ++it) {
        last = generateStatement(*it);
    }
    return last;
}

const Instruction *IRGenerator::generateStatement(const Statement *statement) {
    if (const Constant *el = dynamic_cast<const Constant *>(statement)) {
        return generateConstant(el);
    }
    else if (const Declaration *el = dynamic_cast<const Declaration *>(statement)) {
        return generateDeclaration(el);
    }
    else if (const Expression *el = dynamic_cast<const Expression *>(statement)) {
        return generateExpression(el);
    }
    else if (const Return *el = dynamic_cast<const Return *>(statement)) {
        return generateReturn(el);
    }
    else if (const Variable *el = dynamic_cast<const Variable *>(statement)) {
        return generateVariable(el);
    } else if(const UnExpression *el = dynamic_cast<const UnExpression *>(statement)) {
        return generateUnExpression(el);
    } else {
        assert("Need to handle new types");
    }
    return nullptr;
}

const Instruction *IRGenerator::generateConstant(const Constant *constant) {
    string dest = newTempVar();
    auto instr = new Instruction(cst, to_string(constant->getValue()), dest);
    instructions.push_back(instr);
    return instr;
}

// TODO: changer la manière dont on gère les déclarations
const Instruction *IRGenerator::generateDeclaration(const Declaration *declaration) {
    string dest = newTempVar();
    Instruction *instr = nullptr;
    for (auto it = declaration->getSymbols().begin(); it != declaration->getSymbols().end(); ++it) {
        auto assignement = *it;
        string name = (*it).first;
        Statement *value = (*it).second;
        if (value != nullptr) {
            auto assignStmnt = generateStatement(value);
            string dest = newTempVar();
            instr = new Instruction(load, assignStmnt->dest(), name);
            instructions.push_back(instr);
        }
    }

    return instr;
}

const Instruction *IRGenerator::generateExpression(const Expression *expression) {
    if (expression->getOp().type() == OpType::ASSIGN) {
        if(const Variable *dest = dynamic_cast<const Variable *>(expression->getLeft())) {
            string destName = dest->getName();
            auto rightInstr = generateStatement(expression->getRight());
            auto instr = new Instruction(inst_type::store, rightInstr->dest(), destName);
            instructions.push_back(instr);
            return instr;
        } else {
            assert("la partie droite d'une affectation doit toujours être une variable. La grammaire ne doit pas permettre d'arriver ici");
            return nullptr;
        }
    } else {
        auto leftInstr = generateStatement(expression->getLeft());
        auto rightInstr = generateStatement(expression->getRight());
        string op1 = leftInstr->dest();
        string op2 = rightInstr->dest();
        string dest = newTempVar();
        
        Instruction *inst;
        switch(expression->getOp().type()) {
            case OpType::ADD:
                inst = new Instruction(inst_type::add, op1, dest, op2);
                break;
            case OpType::MINUS:
                inst = new Instruction(inst_type::sub, op1, dest, op2);
                break;
            case OpType::MULT:
                inst = new Instruction(inst_type::mul, op1, dest, op2);
                break;
            case OpType::DIV:
                inst = new Instruction(inst_type::div, op1, dest, op2);
            case OpType::ASSIGN:
                assert("Le cas ASSIGN doit être géré d'une autre manière");
                break;
            default:
                assert("Missing type");
                break;
        }

        instructions.push_back(inst);

        return inst;
    }
}

const Instruction *IRGenerator::generateUnExpression(const UnExpression *expression) {
    auto instr = generateStatement(expression->getExpr());

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
            inst = new Instruction(inst_type::neg, op1, dest);
            break;
        default:
            assert("Missing type");
            break;
    }

    instructions.push_back(inst);

    return inst;
}

const Instruction *IRGenerator::generateReturn(const Return *ret) {
    auto lastInstr = generateStatement(ret->getStatement());
    auto instr = new Instruction(inst_type::ret, lastInstr->dest());
    instructions.push_back(instr);
    return instr;
}

const Instruction *IRGenerator::generateVariable(const Variable *variable) {
    string dest = newTempVar();
    auto instr = new Instruction(load, variable->getName(), dest);
    instructions.push_back(instr);
    return instr;
}

const map<string, int>& IRGenerator::getSymbolTable() {
    return symbolTable;
}

const vector<Instruction*>& IRGenerator::getInstructions() {
    return instructions;
}

string IRGenerator::newTempVar() {
    int offset = stackOffset -= 4;
	string name("0_");
	name.append(to_string(1000 + tempVarCount++));
	symbolTable.emplace(name, offset);

	return name;
}