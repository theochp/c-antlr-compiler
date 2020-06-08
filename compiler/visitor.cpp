// Copied from a file generated from ifcc.g4 by ANTLR 4.7.2


#include "visitor.h"



antlrcpp::Any Visitor::visitAxiom(ifccParser::AxiomContext *ctx) {
	return visitChildren(ctx);
}

antlrcpp::Any Visitor::visitProg(ifccParser::ProgContext *ctx)  {
	for (int i = 0; i < ctx->statement().size(); ++i) {
		visit(ctx->statement(i));
    }

	/*int retval = stoi(ctx->CONST()->getText());
	std::cout<<".globl	main\n"
		" main: \n"
		" 	movl	$"<<retval<<", %eax\n"
		" 	ret\n";*/
	
	return 0;
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
	string name = ctx->NAME()->getText();
	if (symbolTable.find(name) != symbolTable.end()) {
		cout << "This variable already exists" << endl;
	} else {
		symbolTable.emplace(name, lastOffset -= 4);
		cout << "variable " << name << " declared" << endl;

		if (ctx->CONST() != nullptr) {
			cout << "variable " << name << " has value " << ctx->CONST()->getText() << endl;
		}
	}
	return "";
}

antlrcpp::Any Visitor::visitConstExpr(ifccParser::ConstExprContext *ctx) {
	return ctx->CONST()->getText();
}

antlrcpp::Any Visitor::visitVarExpr(ifccParser::VarExprContext *ctx) {
	return "valeur de " + ctx->NAME()->getText();
}

antlrcpp::Any Visitor::visitAffectExpr(ifccParser::AffectExprContext *ctx) {
	string name = ctx->NAME()->getText();
	int offset = this->symbolTable.at(name); //TODO: check if it exists
	string expr = visit(ctx->expr()).as<string>();
	cout<< "Affectation de (resultat de l'expr) dans la variable à l'offset " << offset << endl;
	
	return expr;
}

antlrcpp::Any Visitor::visitRet(ifccParser::RetContext *ctx) {
	cout << "RETURN" << endl;
}

