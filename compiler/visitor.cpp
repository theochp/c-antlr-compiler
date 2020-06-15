// Copied from a file generated from ifcc.g4 by ANTLR 4.7.2
#include <map>

#include "visitor.h"
#include "ast/block.h"
#include "ast/constant.h"
#include "ast/declaration.h"
#include "ir/instruction.h"
#include "ast/symbol.h"
#include "ast/expression.h"
#include "ast/affectation.h"

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
	pair<string, Constant*> declaration;
	declaration.first = name;
 	
	if (ctx->CONST() != nullptr) {
		declaration.second = new Constant(stoi(ctx->CONST()->getText()));
	} else {
		declaration.second = nullptr;
	}
	
	return declaration;
}

antlrcpp::Any Visitor::visitConstExpr(ifccParser::ConstExprContext *ctx) {
	return new Constant(stoi(ctx->CONST()->getText()));
}

antlrcpp::Any Visitor::visitNameExpr(ifccParser::NameExprContext *ctx) {
	return new Symbol(ctx->NAME()->getText());
}

antlrcpp::Any Visitor::visitAffectation(ifccParser::AffectationContext *ctx) {
	string name = ctx->NAME(0)->getText();
	Affectation *affectation = new Affectation();

	affectation->addAffectation(name, visit(ctx->expr()));

	return affectation;
}

antlrcpp::Any Visitor::visitAffectStatement(ifccParser::AffectStatementContext *ctx) {
	return visit(ctx->affectation());
}

antlrcpp::Any Visitor::visitMultExpr(ifccParser::MultExprContext *ctx) {
	return nullptr;
}

antlrcpp::Any Visitor::visitAddExpr(ifccParser::AddExprContext *ctx) {
	return nullptr;
}

antlrcpp::Any Visitor::visitParExpr(ifccParser::ParExprContext *ctx) {
	return nullptr;
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
}

string Visitor::allocateTempVar() {
	int offset = stackOffset -= 4;
	string name("0_"); // on met un 0 au début pour être sur que ça ne correspond à aucun variable c
	name.append(to_string(rand()%1000000+100000));
	symbolTable.emplace(name, offset);

	return name;
}

