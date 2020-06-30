// Copied from a file generated from ifcc.g4 by ANTLR 4.7.2
#include <map>
#include <cassert>

#include "visitor.h"
#include "ast/block.h"
#include "ast/constant.h"
#include "ast/char.h"
#include "ir/instruction.h"
#include "ast/operator.h"
#include "ast/unoperator.h"
#include "ast/assignement.h"
#include "ast/unexpression.h"
#include "ast/return.h"
#include "ast/logicalNot.h"
#include "ast/ifelse.h"
#include "static-analysis/undeclaredVariable.h"
#include "static-analysis/doubleDeclaration.h"
#include "static-analysis/unusedVariable.h"
#include "ast/func.h"
#include "ast/funccall.h"

antlrcpp::Any Visitor::visitAxiom(ifccParser::AxiomContext *ctx) {
	antlrcpp::Any res = visit(ctx->prog()).as<vector<const Node *>>();
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
	vector<const Node *> topLevelNodes;
	for (int i = 0; i < ctx->toplevel().size(); ++i) {
		const Node *topLevelNode = visit(ctx->toplevel(i)).as<Node*>();
		topLevelNodes.push_back(topLevelNode);
	}

	return topLevelNodes;
}

antlrcpp::Any Visitor::visitToplevel(ifccParser::ToplevelContext *ctx) {
	return (Node*) visit(ctx->funcdecl()).as<Func*>();
}

antlrcpp::Any Visitor::visitFuncdecl(ifccParser::FuncdeclContext *ctx) {
	string name = ctx->NAME()->getText();
	symbolTables.emplace(name, map<string, int>());
	symbolOffsets.emplace(name, 0);
	activeSymbolTable = name;
	vector<const FuncParam*> params = visit(ctx->paramDecl()).as<vector<const FuncParam*>>();
	auto func = new Func(name);
	for (auto param : params) {
		symbolTable().emplace(param->getName(), incrementOffset(func->getName(), 4));
		func->addParam(param);
	}
	
	Block *block = visit(ctx->bloc()).as<Block*>();
	func->setBlock(block);

	activeSymbolTable = "!global";

	return func;
}

antlrcpp::Any Visitor::visitParamDecl(ifccParser::ParamDeclContext *ctx) {
	vector<const FuncParam *> params;
	for (int i = 0 ; i < ctx->NAME().size(); ++i) {
		params.push_back(new FuncParam(ctx->NAME(i)->getText()));
	}

	return params;
}

antlrcpp::Any Visitor::visitBloc(ifccParser::BlocContext *ctx) {
	Block *block = new Block();
	for (int i = 0; i < ctx->statement().size(); ++i) {
		auto visited = visit(ctx->statement(i));
		try {
			auto statement = visited.as<Statement*>();
			block->addStatement(statement);
		} catch(bad_cast& e) {
			auto statements = visited.as<vector<Assignement*>>();
			for (auto assign : statements) {
				block->addStatement(assign);
			}
		}
    }
    return block;
}

antlrcpp::Any Visitor::visitExprStatement(ifccParser::ExprStatementContext *ctx) {
	return (Statement*) visit(ctx->expr()).as<Expression*>();
}

antlrcpp::Any Visitor::visitDeclStatement(ifccParser::DeclStatementContext *ctx) {
	return visit(ctx->declaration()).as<vector<Assignement*>>();
}

antlrcpp::Any Visitor::visitRetStatement(ifccParser::RetStatementContext *ctx) {
	return visit(ctx->ret()).as<Statement*>();
}

antlrcpp::Any Visitor::visitIfElseStatement(ifccParser::IfElseStatementContext *ctx) {
    return (Statement*) visit(ctx->ifElse()).as<IfElse*>();
}

antlrcpp::Any Visitor::visitDeclaration(ifccParser::DeclarationContext *ctx) {
	vector<Assignement*> assignements;
	for (int i = 0; i < ctx->individualDeclaration().size(); i++) {
        pair<string, Expression*> symbol = visit(ctx->individualDeclaration(i)).as<pair<string, Expression*>>();
		if (symbol.second != nullptr) {
			assignements.push_back(new Assignement(new Variable(symbol.first), symbol.second));
		}
	}
	return assignements;
}

antlrcpp::Any Visitor::visitIndividualDeclaration(ifccParser::IndividualDeclarationContext *ctx) {
	string name = ctx->NAME()->getText();
	pair<string, Expression*> declaration;
	if (symbolTable().find(name) == symbolTable().end()) {
		declaration.first = name;
		int offset;
        if(Char* c = dynamic_cast<Char*>(declaration.second)){
            offset = incrementOffset(activeSymbolTable, 1);
        } else {
            offset = incrementOffset(activeSymbolTable, 4);
        }
		symbolTable().emplace(name, offset);
        pair<int, int> positionPair = make_pair(ctx->start->getLine(), ctx->start->getCharPositionInLine());
		countUseVar.push_back(make_tuple(name, 0, positionPair));
        if (ctx->expr() != nullptr) {
            Expression* stmnt = visit(ctx->expr()).as<Expression*>();
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
	return (Expression*) new Constant(stoi(ctx->CONST()->getText()));
}

antlrcpp::Any Visitor::visitCharExpr(ifccParser::CharExprContext *ctx) {
    return (Expression*) new Char((int) (ctx->CHAR()->getText().at(1)));
}

antlrcpp::Any Visitor::visitNameExpr(ifccParser::NameExprContext *ctx) {
	string name = ctx->NAME()->getText();

	// watching use of variable
	vector<tuple<string, int, pair<int, int>>>::iterator it =
			std::find_if(countUseVar.begin(), countUseVar.end(), [name](const std::tuple<string, int, pair<int, int>>& e) {return std::get<0>(e) == name;});
	if (it != countUseVar.end())
		get<1>(*it)++;

	if (symbolTable().find(name) == symbolTable().end()) {
		errorCount++;
		UndeclaredVariable* error = new UndeclaredVariable(name, ctx->start->getLine(), ctx->start->getCharPositionInLine());
		errors.push_back(error);
	}

	return (Expression*) new Variable(name);
}

antlrcpp::Any Visitor::visitAffectExpr(ifccParser::AffectExprContext *ctx) {
	string name = ctx->NAME()->getText();
	if (symbolTable().find(name) != symbolTable().end()) {
		Expression * statement = (Expression*) new Assignement(new Variable(name), visit(ctx->expr()).as<Expression*>());
		return statement;
	} else {
		errorCount++;
		UndeclaredVariable* error = new UndeclaredVariable(name, ctx->start->getLine(), ctx->start->getCharPositionInLine());
        errors.push_back(error);
		return (Expression*) nullptr;
	}
}

antlrcpp::Any Visitor::visitMultExpr(ifccParser::MultExprContext *ctx) {
	OpType opType = MULT;
	if (ctx->MULTDIV()->getText() == "/") {
		opType = DIV;
	}
	auto leftExpr = visit(ctx->expr(0));
	auto rightExpr = visit(ctx->expr(1));

	return (Expression*) new Operator(opType, leftExpr, rightExpr);
}

antlrcpp::Any Visitor::visitAddExpr(ifccParser::AddExprContext *ctx) {
	string opStr = ctx->ADDMINUS()->getText();
	
	OpType opType = ADD;
	if (opStr == "-") {
		opType = MINUS;
	}

	auto leftExpr = visit(ctx->expr(0));
	auto rightExpr = visit(ctx->expr(1));

	return (Expression*) new Operator(opType, leftExpr, rightExpr);
}

antlrcpp::Any Visitor::visitUnOp(ifccParser::UnOpContext *ctx) {
	string op = ctx->ADDMINUS()->getText();
	auto expr = visit(ctx->expr()).as<Expression*>();

	if (const Constant * cst = dynamic_cast<const Constant*>(expr)) {
		if (op == "+") {
			return expr;
		} else if(op == "-") {
			// replace by negative value constant
			auto newCst = new Constant(-cst->getValue());
			delete cst;
			return (Expression*) newCst;
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
		return (Expression *) new UnExpression(opType, expr);
	}

	return nullptr;
}

antlrcpp::Any Visitor::visitBitwiseExpr(ifccParser::BitwiseExprContext *ctx) {
    string opStr = ctx->BITWISE()->getText();

    OpType opType = BITWISE_AND;
    if (opStr == "|") {
        opType = BITWISE_OR;
    }
    if (opStr == "^") {
        opType = BITWISE_XOR;
    }

    auto leftExpr = visit(ctx->expr(0));
    auto rightExpr = visit(ctx->expr(1));

    return (Expression*) new Operator(opType, leftExpr, rightExpr);
}

antlrcpp::Any Visitor::visitParExpr(ifccParser::ParExprContext *ctx) {
	return visit(ctx->expr());
}

antlrcpp::Any Visitor::visitCompPrioExpr(ifccParser::CompPrioExprContext *ctx){
	OpType opType = INFCOMP;
	if (ctx->COMP_PRIO()->getText() == "<=") {
		opType = INFEQCOMP;
	}
	else if (ctx->COMP_PRIO()->getText() == ">") {
		opType = SUPCOMP;
	}
	else if (ctx->COMP_PRIO()->getText() == ">=") {
		opType = SUPEQCOMP;
	}
	Expression* leftExpr = visit(ctx->expr(0));
	Expression* rightExpr = visit(ctx->expr(1));

	return (Expression*) new Operator(opType, leftExpr, rightExpr);
}

antlrcpp::Any Visitor::visitCompExpr(ifccParser::CompExprContext *ctx){
	OpType opType = EQUALCOMP;
	if (ctx->COMP()->getText() == "!=") {
		opType = DIFFCOMP;
	}
	Expression* leftExpr = visit(ctx->expr(0));
	Expression* rightExpr = visit(ctx->expr(1));

	return (Expression*) new Operator(opType, leftExpr, rightExpr);
}

antlrcpp::Any Visitor::visitRet(ifccParser::RetContext *ctx) {
	return (Statement*) new Return(visit(ctx->expr()).as<Expression*>());
}

antlrcpp::Any Visitor::visitFuncall(ifccParser::FuncallContext *ctx) {
	string name = ctx->NAME()->getText();
	auto paramList = visit(ctx->paramList()).as<const vector<Expression*>&>();
	FuncCall *fCall = new FuncCall(name);
	fCall->addParamStatements(paramList);
	return (Expression *) fCall;
}

antlrcpp::Any Visitor::visitParamList(ifccParser::ParamListContext *ctx) {
	vector<Expression*> statements;

	for (int i = 0; i < ctx->expr().size(); ++i) {
		auto statement = visit(ctx->expr(i)).as<Expression*>();
		statements.push_back(statement);
	}

	return statements;
}

antlrcpp::Any Visitor::visitParam(ifccParser::ParamContext *ctx) {
	// TODO: for declarations
}

antlrcpp::Any Visitor::visitNotExpr(ifccParser::NotExprContext *ctx){
	string op = ctx->NOT()->getText();
    auto expr = visit(ctx->expr()).as<Expression*>();

    if (op == "!") {
        return (Expression *) new LogicalNot(expr);
    } else if (op == "~") {
        return (Expression *) new UnExpression(UnOpType::BITWISE_NOT, expr);
    } else {
        assert("Need to handle new op");
    }

    return nullptr;
}

antlrcpp::Any Visitor::visitIfElse(ifccParser::IfElseContext *ctx) {
	Block *ifBlock = visit(ctx->blocOrStatement()).as<Block *>();
	Block *elseBlock = nullptr;
	if (ctx->elsePart()) {
		elseBlock = visit(ctx->elsePart()).as<Block*>();
	}
	Expression *condition = visit(ctx->expr()).as<Expression*>();

	return new IfElse(ifBlock, elseBlock, condition);
}

antlrcpp::Any Visitor::visitElsePart(ifccParser::ElsePartContext *ctx) {
	return visit(ctx->blocOrStatement()).as<Block*>();
}

antlrcpp::Any Visitor::visitBlocOrStatement(ifccParser::BlocOrStatementContext *ctx) {
	if (ctx->bloc()) {
		return visit(ctx->bloc()).as<Block*>();
	} else {
		auto block = new Block();
		block->addStatement(visit(ctx->statement()).as<Statement*>());
		return block;
	}
}
