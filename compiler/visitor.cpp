// Copied from a file generated from ifcc.g4 by ANTLR 4.7.2
#include <map>
#include <assert.h>

#include "visitor.h"
#include "ast/block.h"
#include "ast/constant.h"
#include "ast/declaration.h"
#include "ir/instruction.h"
#include "ast/operator.h"
#include "ast/unoperator.h"
#include "ast/assignement.h"
#include "ast/expression.h"
#include "ast/unexpression.h"
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
        auto symbol = visit(ctx->individualDeclaration(i)).as<pair<string, Statement*>>();
		declaration->addSymbol(symbol.first, symbol.second);
	}
	return declaration;
}

antlrcpp::Any Visitor::visitIndividualDeclaration(ifccParser::IndividualDeclarationContext *ctx) {
	string name = ctx->NAME()->getText();
	if (symbolTable.find(name) == symbolTable.end()) {
		pair<string, Statement*> declaration;
		declaration.first = name;
		int offset = stackOffset -= 4;
		symbolTable.emplace(name, offset);
		if (ctx->expr() != nullptr) {
			auto stmnt = visit(ctx->expr()).as<Statement*>();
			declaration.second = stmnt;
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

antlrcpp::Any Visitor::visitUnOp(ifccParser::UnOpContext *ctx) {
	string op = ctx->ADDMINUS()->getText();
	auto expr = visit(ctx->expr()).as<Statement*>();

	if (const Constant * cst = dynamic_cast<const Constant*>(expr)) {
		if (op == "+") {
			return expr;
		} else if(op == "-") {
			// replace by negative value constant
			auto newCst = new Constant(-cst->getValue());
			delete cst;
			return (Statement*) newCst;
		} else {
			assert("Need to handle new op");
		}
	} else {
		UnOpType opType;
		if (op == "+") {
			opType = UnOpType::UN_PLUS;
		} else if (op == "-") {
			opType = UnOpType::UN_MINUS;
		} else {
			assert("Need to handle new op");
		}
		return (Statement *) new UnExpression(opType, expr);
	}

	return nullptr;
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

