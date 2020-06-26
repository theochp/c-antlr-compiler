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
#include "ast/logicalNot.h"
#include "static-analysis/undeclaredVariable.h"
#include "static-analysis/doubleDeclaration.h"
#include "static-analysis/unusedVariable.h"
#include "ast/func.h"

#define INDENT "\t"


antlrcpp::Any Visitor::visitAxiom(ifccParser::AxiomContext *ctx) {
	antlrcpp::Any res = visit(ctx->prog()).as<Node*>();
    for (tuple<string, int, pair<int, int>> varData : countUseVar){
        if (get<1>(varData) == 0){
            warnings.push_back(new UnusedVariable(get<0>(varData), get<2>(varData).first, get<2>(varData).second));
            warningCount++;
        }
    }
    return res;
}

antlrcpp::Any Visitor::visitProg(ifccParser::ProgContext *ctx)  {
	// TODO : handle many top level blocks
	return visit(ctx->toplevel(0)).as<Node*>();
}

antlrcpp::Any Visitor::visitToplevel(ifccParser::ToplevelContext *ctx) {
	return (Node*) visit(ctx->funcdecl()).as<Func*>();
}

antlrcpp::Any Visitor::visitFuncdecl(ifccParser::FuncdeclContext *ctx) {
	Block *block = visit(ctx->bloc()).as<Block*>();
	string name = ctx->NAME()->getText();
	return new Func(name, block);
}

antlrcpp::Any Visitor::visitBloc(ifccParser::BlocContext *ctx) {
	Block *block = new Block();
	for (int i = 0; i < ctx->statement().size(); ++i) {
		auto statement = visit(ctx->statement(i)).as<Statement*>();
		block->addStatement(statement);
    }
    return block;
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
        pair<string, Statement*> symbol = visit(ctx->individualDeclaration(i)).as<pair<string, Statement*>>();
        if (symbol.first != ""){
            declaration->addSymbol(symbol.first, symbol.second);
        }
	}
	return declaration;
}

antlrcpp::Any Visitor::visitIndividualDeclaration(ifccParser::IndividualDeclarationContext *ctx) {
	string name = ctx->NAME()->getText();
	pair<string, Statement*> declaration;
	if (symbolTable.find(name) == symbolTable.end()) {
		declaration.first = name;
		int offset = stackOffset -= 4;
		symbolTable.emplace(name, offset);
        pair<int, int> positionPair = make_pair(ctx->start->getLine(), ctx->start->getCharPositionInLine());
		countUseVar.push_back(make_tuple(name, 0, positionPair));
        if (ctx->expr() != nullptr) {
            Statement* stmnt = visit(ctx->expr()).as<Statement*>();
            declaration.second = stmnt;
        } else {
            declaration.second = nullptr;
        }
	} else {
		errorCount++;
		DoubleDeclaration* error = new DoubleDeclaration(name, ctx->start->getLine(), ctx->start->getCharPositionInLine());
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
	string name = ctx->NAME()->getText();

	// watching use of variable
	vector<tuple<string, int, pair<int, int>>>::iterator it =
			std::find_if(countUseVar.begin(), countUseVar.end(), [name](const std::tuple<string, int, pair<int, int>>& e) {return std::get<0>(e) == name;});
	if (it != countUseVar.end())
		get<1>(*it)++;

	if (symbolTable.find(name) == symbolTable.end()) {
		errorCount++;
		UndeclaredVariable* error = new UndeclaredVariable(name, ctx->start->getLine(), ctx->start->getCharPositionInLine());
		errors.push_back(error);
	}

	return (Statement*) new Variable(name);
}

antlrcpp::Any Visitor::visitAffectExpr(ifccParser::AffectExprContext *ctx) {
	string name = ctx->NAME()->getText();
	if (symbolTable.find(name) != symbolTable.end()) {
		Statement * statement = (Statement*) new Assignement(new Variable(name), visit(ctx->expr()).as<Statement*>());
		return statement;
	} else {
		errorCount++;
		UndeclaredVariable* error = new UndeclaredVariable(name, ctx->start->getLine(), ctx->start->getCharPositionInLine());
        errors.push_back(error);
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

antlrcpp::Any Visitor::visitBitwiseExpr(ifccParser::BitwiseExprContext *ctx) {
    string opStr = ctx->BITWISE()->getText();

    Operator opType = BITWISE_AND;
    if (opStr == "|") {
        opType = BITWISE_OR;
    }
    if (opStr == "^") {
        opType = BITWISE_XOR;
    }

    auto leftExpr = visit(ctx->expr(0));
    auto rightExpr = visit(ctx->expr(1));

    return (Statement*) new Expression(opType, leftExpr, rightExpr);
}

antlrcpp::Any Visitor::visitParExpr(ifccParser::ParExprContext *ctx) {
	return visit(ctx->expr());
}

antlrcpp::Any Visitor::visitCompPrioExpr(ifccParser::CompPrioExprContext *ctx){
	Operator opType = INFCOMP;
	if (ctx->COMP_PRIO()->getText() == "<=") {
		opType = INFEQCOMP;
	}
	else if (ctx->COMP_PRIO()->getText() == ">") {
		opType = SUPCOMP;
	}
	else if (ctx->COMP_PRIO()->getText() == ">=") {
		opType = SUPEQCOMP;
	}
	Statement* leftExpr = visit(ctx->expr(0));
	Statement* rightExpr = visit(ctx->expr(1));

	return (Statement*) new Expression(opType, leftExpr, rightExpr);
}

antlrcpp::Any Visitor::visitCompExpr(ifccParser::CompExprContext *ctx){
	Operator opType = EQUALCOMP;
	if (ctx->COMP()->getText() == "!=") {
		opType = DIFFCOMP;
	}
	Statement* leftExpr = visit(ctx->expr(0));
	Statement* rightExpr = visit(ctx->expr(1));

	return (Statement*) new Expression(opType, leftExpr, rightExpr);
}

antlrcpp::Any Visitor::visitRet(ifccParser::RetContext *ctx) {
	return (Statement*) new Return(visit(ctx->expr()).as<Statement*>());
}

antlrcpp::Any Visitor::visitNotExpr(ifccParser::NotExprContext *ctx){
    string op = ctx->NOT()->getText();
    auto expr = visit(ctx->expr()).as<Statement*>();

    if (op == "!") {
        return (Statement *) new LogicalNot(expr);
    } else if (op == "~") {
        return (Statement *) new UnExpression(UnOpType::BITWISE_NOT, expr);
    } else {
        assert("Need to handle new op");
    }

    return nullptr;
}

string Visitor::allocateTempVar() {
	int offset = stackOffset -= 4;
	string name("0_"); // on met un 0 au début pour être sur que ça ne correspond à aucun variable c
	name.append(to_string(rand()%1000000+100000));
	symbolTable.emplace(name, offset);

	return name;
}

