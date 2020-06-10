// Copied from a file generated from ifcc.g4 by ANTLR 4.7.2


#include "visitor.h"

#define INDENT "\t"


antlrcpp::Any Visitor::visitAxiom(ifccParser::AxiomContext *ctx) {
	return visit(ctx->prog()).as<string>();
}

antlrcpp::Any Visitor::visitProg(ifccParser::ProgContext *ctx)  {
	stringstream result;
	result << ".globl  main" << endl;
	result << "main:" << endl;
	result << INDENT << "pushq   %rbp" << endl;
	result << INDENT << "movq    %rsp, %rbp" << endl;
	for (int i = 0; i < ctx->statement().size(); ++i) {
		result << INDENT << visit(ctx->statement(i)).as<string>() << endl;
    }
	
	return result.str();
}

antlrcpp::Any Visitor::visitExprStatement(ifccParser::ExprStatementContext *ctx) {
	return visit(ctx->expr()).as<string>();
}

antlrcpp::Any Visitor::visitDeclStatement(ifccParser::DeclStatementContext *ctx) {
	return visit(ctx->declaration()).as<string>();
}

antlrcpp::Any Visitor::visitRetStatement(ifccParser::RetStatementContext *ctx) {
	return visit(ctx->ret()).as<string>();
}

antlrcpp::Any Visitor::visitDeclaration(ifccParser::DeclarationContext *ctx) {
	string name = ctx->NAME()->getText();
	if (this->symbolTable.find(name) != this->symbolTable.end()) {
		cout << "This variable already exists" << endl;
		errorCount++;
	} else {
		int offset = this->lastOffset -= 4;
		this->symbolTable.emplace(name, offset);
	
		if (ctx->CONST() != nullptr) {
			return "movl    $" + ctx->CONST()->getText() + ", " + to_string(offset) + "(%rbp)";
		}
	}
	return string("");
}

antlrcpp::Any Visitor::visitConstExpr(ifccParser::ConstExprContext *ctx) {
	return "movl   $" + ctx->CONST()->getText() + ", %eax";
}

antlrcpp::Any Visitor::visitVarExpr(ifccParser::VarExprContext *ctx) {
	string name = ctx->NAME()->getText(); 
	try {
		int offset = this->symbolTable.at(name);
		return "movl   " + to_string(offset) + "(%rbp), %eax";
	} catch (const out_of_range& ex) {
		cout << "Use of undefined variable " + name << endl;
		errorCount++;
		return string("");
	}
}

antlrcpp::Any Visitor::visitAffectExpr(ifccParser::AffectExprContext *ctx) {
	string name = ctx->NAME()->getText();

	try {
		stringstream result;
		int offset = this->symbolTable.at(name);
		string exprAsm = visit(ctx->expr()).as<string>();
		result << exprAsm << endl;
		result << INDENT << "movl   %eax, " << to_string(offset) << "(%rbp)";

		return result.str();
	} catch (const out_of_range& ex) {
		cout << "Use of undefined variable " + name << endl;
		errorCount++;
		return string("");
	}
}

antlrcpp::Any Visitor::visitRet(ifccParser::RetContext *ctx) {
	stringstream result;

	if (ctx->expr() != nullptr) {
		result << visit(ctx->expr()).as<string>() << endl << INDENT;
	}

	result << "popq    %rbp" << endl 
		   << INDENT << "ret";

	return result.str();
}

