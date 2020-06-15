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

#define INDENT "\t"


antlrcpp::Any Visitor::visitAxiom(ifccParser::AxiomContext *ctx) {
	return visit(ctx->prog());
}

antlrcpp::Any Visitor::visitProg(ifccParser::ProgContext *ctx)  {
	return visit(ctx->bloc());
}

antlrcpp::Any Visitor::visitBloc(ifccParser::BlocContext *ctx) {
	Block *block = new Block();
	for (int i = 0; i < ctx->statement().size(); ++i) {
		block->addNode(visit(ctx->statement(i)));
    }
    return block;
}

antlrcpp::Any Visitor::visitExprStatement(ifccParser::ExprStatementContext *ctx) {
	return visit(ctx->expr());
}

antlrcpp::Any Visitor::visitDeclStatement(ifccParser::DeclStatementContext *ctx) {
	return visit(ctx->declaration());
}

antlrcpp::Any Visitor::visitRetStatement(ifccParser::RetStatementContext *ctx) {
	return visit(ctx->ret());
}

antlrcpp::Any Visitor::visitDeclaration(ifccParser::DeclarationContext *ctx) {
	Declaration *declaration = new Declaration();
	for (int i = 0; i < ctx->individualDeclaration().size(); i++) {
        auto symbol = visit(ctx->individualDeclaration(i)).as<pair<string, Constant*>>();
		declaration->addSymbol(symbol.first, symbol.second);
	}
	return declaration;
}

antlrcpp::Any Visitor::visitIndividualDeclaration(ifccParser::IndividualDeclarationContext *ctx) {
	string name = ctx->NAME()->getText();

	if (symbolTable.find(name) == symbolTable.end()) {
		pair<string, Constant*> declaration;
		declaration.first = name;
		
		if (ctx->CONST() != nullptr) {
			declaration.second = new Constant(stoi(ctx->CONST()->getText()));
		} else {
			declaration.second = nullptr;
		}
		
		return declaration;
	} else {
		errorCount++;
		cerr << "ERR: Déclaration d'une variable qui existe déjà" << endl;
	}

	return nullptr;
}

antlrcpp::Any Visitor::visitConstExpr(ifccParser::ConstExprContext *ctx) {
	return new Constant(stoi(ctx->CONST()->getText()));
}

antlrcpp::Any Visitor::visitNameExpr(ifccParser::NameExprContext *ctx) {
	return new Variable(ctx->NAME()->getText());
}

antlrcpp::Any Visitor::visitAffectExpr(ifccParser::AffectExprContext *ctx) {
	string name = ctx->NAME()->getText();
	if (symbolTable.find(name) != symbolTable.end()) {
		return new Assignement(new Variable(name), visit(ctx->expr()));
	} else {
		cerr << "ERR: Use of undefined variable " + name << endl;
		errorCount++;
	}
	
	return nullptr;
}

antlrcpp::Any Visitor::visitMultExpr(ifccParser::MultExprContext *ctx) {
	Operator opType = MULT;
	if (ctx->MULTDIV()->getText() == "/") {
		opType = DIV;
	}

	Expression* leftExpr = (Expression*) visit(ctx->expr(0));
	Expression* rightExpr = (Expression*) visit(ctx->expr(1));

	return new Expression(opType, leftExpr, rightExpr);
}

antlrcpp::Any Visitor::visitAddExpr(ifccParser::AddExprContext *ctx) {
	Operator opType = ADD;
	if (ctx->ADDMINUS()->getText() == "-") {
		opType = MINUS;
	}

	Expression* leftExpr = (Expression*) visit(ctx->expr(0));
    Expression* rightExpr = (Expression*) visit(ctx->expr(1));

	return new Expression(opType, leftExpr, rightExpr);
}

antlrcpp::Any Visitor::visitParExpr(ifccParser::ParExprContext *ctx) {
	return visit(ctx->expr());
}

antlrcpp::Any Visitor::visitRet(ifccParser::RetContext *ctx) {
	/*if (ctx->expr() != nullptr) {
		visit(ctx->expr());
		string source = instructions.back()->dest();
		auto inst = new instruction(ret, source);
		instructions.push_back(inst);
		return inst;
	} else {
		auto inst = new instruction(ret);
		instructions.push_back(inst);
		return inst;
	}*/
	return nullptr;
}

string Visitor::allocateTempVar() {
	int offset = stackOffset -= 4;
	string name("0_"); // on met un 0 au début pour être sur que ça ne correspond à aucun variable c
	name.append(to_string(rand()%1000000+100000));
	symbolTable.emplace(name, offset);

	return name;
}

