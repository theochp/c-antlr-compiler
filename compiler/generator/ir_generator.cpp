#include "ir_generator.h"

IRGenerator::IRGenerator(Node *ast, map<string, int> symbolTable, int stackOffset)
    : ast(ast), symbolTable(symbolTable), stackOffset() {

}

void IRGenerator::generate() {
    if (const Block *block = dynamic_cast<const Block *>(ast)) {
        generateBlock(block);
    }
}

const instruction *IRGenerator::generateBlock(const Block *block) {
    const instruction *last;
    for (auto it = block->getStatements().begin(); it != block->getStatements().end(); ++it) {
        last = generateStatement(*it);
    }
    return last;
}

const instruction *IRGenerator::generateStatement(const Statement *statement) {
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
    }
}

const instruction *IRGenerator::generateConstant(const Constant *constant) {
    string dest = newTempVar();
    auto instr = new instruction(cst, to_string(constant->getValue()), dest);
    instructions.push_back(instr);
    return instr;
}

// TODO: changer la manière dont on gère les déclarations
const instruction *IRGenerator::generateDeclaration(const Declaration *declaration) {
    string dest = newTempVar();
    instruction *instr = nullptr;
    for (auto it = declaration->getSymbols().begin(); it != declaration->getSymbols().end(); ++it) {
        auto assignement = *it;
        string name = (*it).first;
        Constant *value = (*it).second;
        if (value != nullptr) {
            auto constInstr = generateConstant(value);
            string dest = newTempVar();
            instr = new instruction(load, constInstr->dest(), name);
            instructions.push_back(instr);
        }
    }

    return instr;
}

const instruction *IRGenerator::generateExpression(const Expression *expression) {
    // TODO: handle ASSIGN differentl
    auto leftInstr = generateStatement(expression->getLeft());
    auto rightInstr = generateStatement(expression->getRight());
    string op1 = leftInstr->dest();
    string op2 = rightInstr->dest();
    string dest = newTempVar();
    
    instruction *inst;
    switch(expression->getOp().type()) {
        case ADD:
            inst = new instruction(inst_type::add, op1, dest, op2);
            break;
        case MINUS:
            inst = new instruction(inst_type::sub, op1, dest, op2);
            break;
        case MULT:
            inst = new instruction(inst_type::mul, op1, dest, op2);
            break;
        case DIV:
            inst = new instruction(inst_type::div, op1, dest, op2);
        case ASSIGN:
            // TODO
            break;
    }

    instructions.push_back(inst);

    return inst;
}

const instruction *IRGenerator::generateReturn(const Return *ret) {
    auto lastInstr = generateStatement(ret->getStatement());
    auto instr = new instruction(inst_type::ret, lastInstr->dest());
    instructions.push_back(instr);
    return instr;
}

const instruction *IRGenerator::generateVariable(const Variable *variable) {
    string dest = newTempVar();
    auto instr = new instruction(load, variable->getName(), dest);
    instructions.push_back(instr);
    return instr;
}

const map<string, int>& IRGenerator::getSymbolTable() {
    return symbolTable;
}

const vector<instruction*>& IRGenerator::getInstructions() {
    return instructions;
}

string IRGenerator::newTempVar() {
    int offset = stackOffset -= 4;
	string name("0_");
	name.append(to_string(1000 + tempVarCount++));
	symbolTable.emplace(name, offset);

	return name;
}