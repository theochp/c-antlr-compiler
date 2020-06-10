// Copied from a file generated from ifcc.g4 by ANTLR 4.7.2

#include "visitor.h"
#include "ir/instruction.h"

#define INDENT "\t"


antlrcpp::Any Visitor::visitAxiom(ifccParser::AxiomContext *ctx) {
	return visit(ctx->prog());
}

antlrcpp::Any Visitor::visitProg(ifccParser::ProgContext *ctx)  {
	stringstream result;
	result << ".globl  main" << endl;
	result << "main:" << endl;
	result << INDENT << "pushq   %rbp" << endl;
	result << INDENT << "movq    %rsp, %rbp" << endl;
	
	visit(ctx->bloc());
	
	return result.str();
}

antlrcpp::Any Visitor::visitBloc(ifccParser::BlocContext *ctx){
	for (int i = 0; i < ctx->statement().size(); ++i) {
		visit(ctx->statement(i));
    }
    return "";
}

antlrcpp::Any Visitor::visitExprStatement(ifccParser::ExprStatementContext *ctx) {
	visit(ctx->expr());
	return "";
}

antlrcpp::Any Visitor::visitDeclStatement(ifccParser::DeclStatementContext *ctx) {
	visit(ctx->declaration());
	return "";
}

antlrcpp::Any Visitor::visitRetStatement(ifccParser::RetStatementContext *ctx) {
	visit(ctx->ret());
	return "";
}

antlrcpp::Any Visitor::visitDeclaration(ifccParser::DeclarationContext *ctx) {
	instruction* inst = nullptr;
	for (int i = 0; i<ctx->individualDeclaration().size(); i++){
        auto retInst = visit(ctx->individualDeclaration(i));
		if (retInst != nullptr) {
			inst = retInst;
		}
	}
	return inst;
}

antlrcpp::Any Visitor::visitIndividualDeclaration(ifccParser::IndividualDeclarationContext *ctx) {
	string name = ctx->NAME()->getText();
	if (this->symbolTable.find(name) != this->symbolTable.end()) {
		cout << "This variable already exists" << endl;
		errorCount++;
	} else {
		int offset = this->stackOffset -= 4;
		this->symbolTable.emplace(name, offset);
		if (ctx->CONST() != nullptr) { // if a const is given, we affect that in memory to variable
			auto inst = new instruction(cst, ctx->CONST()->getText(), name);
			instructions.push_back(inst);
			return inst;
		}
	}
	return nullptr;
}


antlrcpp::Any Visitor::visitConstExpr(ifccParser::ConstExprContext *ctx) {
	string addr = allocateTempVar();
	auto inst = new instruction(cst, ctx->CONST()->getText(), addr);
	instructions.push_back(inst);
	return inst; // do we need a return ?
}

antlrcpp::Any Visitor::visitVarExpr(ifccParser::VarExprContext *ctx) {
	string name = ctx->NAME()->getText(); 
	try {
		int offset = this->symbolTable.at(name); // on laisse cette ligne pour check si la variable existe
		string addr = allocateTempVar();
		auto inst = new instruction(load, name, addr);
		instructions.push_back(inst);
		return inst;
	} catch (const out_of_range& ex) {
		cout << "Use of undefined variable " + name << endl;
		errorCount++;
		return nullptr;
	}
}

antlrcpp::Any Visitor::visitAffectExpr(ifccParser::AffectExprContext *ctx) {
	string name = ctx->NAME()->getText();
	try {
		int offset = this->symbolTable.at(name);  // on laisse cette ligne pour check si la variable existe
		visit(ctx->expr());
		string source = instructions.back()->dest(); // on récupère la destination de la précédente instruction
		auto inst = new instruction(store, source, name);
		instructions.push_back(inst);
		return inst;
	} catch (const out_of_range& ex) {
		cout << "Use of undefined variable " + name << endl;
		errorCount++;
	}

	return nullptr;
}

antlrcpp::Any Visitor::visitRet(ifccParser::RetContext *ctx) {
	if (ctx->expr() != nullptr) {
		visit(ctx->expr());
		string source = instructions.back()->dest();
		auto inst = new instruction(ret, source);
		instructions.push_back(inst);
		return inst;
	} else {
		auto inst = new instruction(ret);
		instructions.push_back(inst);
		return inst;
	}
}

string Visitor::allocateTempVar() {
	int offset = stackOffset -= 4;
	string name("0_"); // on met un 0 au début pour être sur que ça ne correspond à aucun variable c
	name.append(to_string(rand()%1000000+100000));
	symbolTable.emplace(name, offset);

	return name;
}

