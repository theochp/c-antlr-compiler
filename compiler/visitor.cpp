// Copied from a file generated from ifcc.g4 by ANTLR 4.7.2
#include <map>

#include "visitor.h"
#include "ast/block.h"
#include "ast/constant.h"
#include "ast/declaration.h"
#include "ir/instruction.h"
#include "ast/operator.h"
#include "ast/affectation.h"
#include "ast/assignement.h"
#include "ast/expression.h"
#include "ast/return.h"

#define INDENT "\t"


antlrcpp::Any Visitor::visitAxiom(ifccParser::AxiomContext *ctx) {
	return visit(ctx->prog()).as<Node*>();
}

antlrcpp::Any Visitor::visitProg(ifccParser::ProgContext *ctx)  {
	return visit(ctx->bloc()).as<Node*>();
}

antlrcpp::Any Visitor::visitBloc(ifccParser::BlocContext *ctx) {
	Block *block = new Block();
	for (int i = 0; i < ctx->statement().size(); ++i) {
		auto statement = visit(ctx->statement(i)).as<Statement*>();
		block->addStatement(statement);
    }
    return (Node*) block;
}

antlrcpp::Any Visitor::visitExprStatement(ifccParser::ExprStatementContext *ctx) {
	return visit(ctx->expr()).as<Statement*>();
}

antlrcpp::Any Visitor::visitDeclStatement(ifccParser::DeclStatementContext *ctx) {
	auto decl = visit(ctx->declaration()).as<Declaration*>();
	return (Statement*) decl;
}

antlrcpp::Any Visitor::visitRetStatement(ifccParser::RetStatementContext *ctx) {
	return visit(ctx->ret()).as<Statement*>();
}

antlrcpp::Any Visitor::visitDeclaration(ifccParser::DeclarationContext *ctx) {
	Declaration *declaration = new Declaration();
	for (int i = 0; i < ctx->individualDeclaration().size(); i++) {
        pair<string, Constant*> symbol = visit(ctx->individualDeclaration(i)).as<pair<string, Constant*>>();
        if (symbol.first != ""){
			declaration->addSymbol(symbol.first, symbol.second);
		}
	}
	return declaration;
}

antlrcpp::Any Visitor::visitIndividualDeclaration(ifccParser::IndividualDeclarationContext *ctx) {
	string name = ctx->NAME()->getText();
	pair<string, Constant*> declaration;
	if (symbolTable.find(name) == symbolTable.end()) {
		declaration.first = name;
		int offset = stackOffset -= 4;
		symbolTable.emplace(name, offset);
		if (ctx->CONST() != nullptr) {
			declaration.second = new Constant(stoi(ctx->CONST()->getText()));
		} else {
			declaration.second = nullptr;
		}
	} else {
		warningCount++;
		doubleDeclaration* error = new doubleDeclaration(name);
		errors.push_back(error);
		declaration.first = "";
		declaration.second = nullptr;
	}
    return declaration;
}

antlrcpp::Any Visitor::visitConstExpr(ifccParser::ConstExprContext *ctx) {
	return (Statement*) new Constant(stoi(ctx->CONST()->getText()));
}

antlrcpp::Any Visitor::visitNameExpr(ifccParser::NameExprContext *ctx) {
	return (Statement*) new Variable(ctx->NAME()->getText());
}

antlrcpp::Any Visitor::visitAffectExpr(ifccParser::AffectExprContext *ctx) {
	string name = ctx->NAME()->getText();
	if (symbolTable.find(name) != symbolTable.end()) {
		Statement * statement = (Statement*) new Assignement(new Variable(name), visit(ctx->expr()).as<Statement*>());
		return statement;
	} else {
		cerr << "ERR: Use of undefined variable " + name << endl;
		errorCount++;
		return (Statement*) nullptr;
	}
}

antlrcpp::Any Visitor::visitMultExpr(ifccParser::MultExprContext *ctx) {
	Operator opType = MULT;
	if (ctx->MULTDIV()->getText() == "/") {
		opType = DIV;
	}
	auto leftExpr = visit(ctx->expr(0));
	auto rightExpr = visit(ctx->expr(1));

	return (Statement*) new Expression(opType, leftExpr, rightExpr);
}

antlrcpp::Any Visitor::visitAddExpr(ifccParser::AddExprContext *ctx) {
	string opStr = ctx->ADDMINUS()->getText();
	
	Operator opType = ADD;
	if (opStr == "-") {
		opType = MINUS;
	}

	auto leftExpr = visit(ctx->expr(0));
	auto rightExpr = visit(ctx->expr(1));

	return (Statement*) new Expression(opType, leftExpr, rightExpr);
}

antlrcpp::Any Visitor::visitParExpr(ifccParser::ParExprContext *ctx) {
	return visit(ctx->expr());
}

antlrcpp::Any Visitor::visitRet(ifccParser::RetContext *ctx) {
	return (Statement*) new Return(visit(ctx->expr()).as<Statement*>());
}

string Visitor::allocateTempVar() {
	int offset = stackOffset -= 4;
	string name("0_"); // on met un 0 au début pour être sur que ça ne correspond à aucun variable c
	name.append(to_string(rand()%1000000+100000));
	symbolTable.emplace(name, offset);

	return name;
}

