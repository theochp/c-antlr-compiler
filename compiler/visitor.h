
// Copied from a file generated from ifcc.g4 by ANTLR 4.7.2

#pragma once

#include <map>
#include "antlr4-runtime.h"
#include "antlr4-generated/ifccVisitor.h"
#include "ir/instruction.h"

using namespace std;

/**
 * This class provides an empty implementation of ifccVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  Visitor : public ifccVisitor {
public:
	virtual antlrcpp::Any visitAxiom(ifccParser::AxiomContext *ctx) override;
	virtual antlrcpp::Any visitProg(ifccParser::ProgContext *ctx) override;
    virtual antlrcpp::Any visitBloc(ifccParser::BlocContext *ctx) override;
	virtual antlrcpp::Any visitExprStatement(ifccParser::ExprStatementContext *ctx) override;

    virtual antlrcpp::Any visitDeclStatement(ifccParser::DeclStatementContext *ctx) override;

    virtual antlrcpp::Any visitRetStatement(ifccParser::RetStatementContext *ctx) override;

    virtual antlrcpp::Any visitDeclaration(ifccParser::DeclarationContext *ctx) override;

	virtual antlrcpp::Any visitIndividualDeclaration(ifccParser::IndividualDeclarationContext *ctx) override;

	virtual antlrcpp::Any visitVarExpr(ifccParser::VarExprContext *ctx) override;

    virtual antlrcpp::Any visitConstExpr(ifccParser::ConstExprContext *ctx) override;

    virtual antlrcpp::Any visitAffectExpr(ifccParser::AffectExprContext *ctx) override;

    virtual antlrcpp::Any visitRet(ifccParser::RetContext *ctx) override;

    string allocateTempVar();

    int getErrCount() {
        return errorCount;
    }

    map<string, int> getSymbolTable() {
        return symbolTable;
    }

    vector<instruction *> getInstructions() {
        return instructions;
    }

    ~Visitor() {
        for (auto it = instructions.begin(); it != instructions.end(); ++it) {
            delete *it;
        }
    }
private:
	map<string, int> symbolTable;
    vector<instruction *> instructions;
	int stackOffset  = 0;
    int errorCount = 0;
};

