
// Copied from a file generated from ifcc.g4 by ANTLR 4.7.2

#pragma once

#include <map>
#include "antlr4-runtime.h"
#include "antlr4-generated/ifccVisitor.h"
#include "ir/instruction.h"
#include "static-analysis/error.h"
#include "static-analysis/warning.h"

using namespace std;

/**
 * This class provides an empty implementation of ifccVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  Visitor : public ifccVisitor {
public:
	virtual antlrcpp::Any visitAxiom(ifccParser::AxiomContext *ctx) override;
    
	virtual antlrcpp::Any visitProg(ifccParser::ProgContext *ctx) override;

    virtual antlrcpp::Any visitToplevel(ifccParser::ToplevelContext *ctx) override;

    virtual antlrcpp::Any visitFuncdecl(ifccParser::FuncdeclContext *ctx) override;

    virtual antlrcpp::Any visitParamDecl(ifccParser::ParamDeclContext *ctx) override;

    virtual antlrcpp::Any visitBloc(ifccParser::BlocContext *ctx) override;

	virtual antlrcpp::Any visitExprStatement(ifccParser::ExprStatementContext *ctx) override;

    virtual antlrcpp::Any visitDeclStatement(ifccParser::DeclStatementContext *ctx) override;

    virtual antlrcpp::Any visitRetStatement(ifccParser::RetStatementContext *ctx) override;

    virtual antlrcpp::Any visitDeclaration(ifccParser::DeclarationContext *ctx) override;

	virtual antlrcpp::Any visitValueDeclaration(ifccParser::ValueDeclarationContext *ctx) override;

	virtual antlrcpp::Any visitNameExpr(ifccParser::NameExprContext *ctx) override;

    virtual antlrcpp::Any visitConstExpr(ifccParser::ConstExprContext *ctx) override;

    virtual antlrcpp::Any visitMultExpr(ifccParser::MultExprContext *ctx) override;

    virtual antlrcpp::Any visitAddExpr(ifccParser::AddExprContext *ctx) override;

    virtual antlrcpp::Any visitParExpr(ifccParser::ParExprContext *ctx) override;

    virtual antlrcpp::Any visitBitwiseExpr(ifccParser::BitwiseExprContext *context) override;

    virtual antlrcpp::Any visitAffectExpr(ifccParser::AffectExprContext *ctx) override;

    virtual antlrcpp::Any visitCompPrioExpr(ifccParser::CompPrioExprContext *ctx) override;

    virtual antlrcpp::Any visitCompExpr(ifccParser::CompExprContext *ctx) override;

    virtual antlrcpp::Any visitUnOp(ifccParser::UnOpContext *ctx) override;

    virtual antlrcpp::Any visitRet(ifccParser::RetContext *ctx) override;

    virtual antlrcpp::Any visitArrayDeclaration(ifccParser::ArrayDeclarationContext *ctx) override;

    virtual antlrcpp::Any visitArrayDeclarationAssignation(ifccParser::ArrayDeclarationAssignationContext *ctx) override;

    virtual antlrcpp::Any visitArrayAssignation(ifccParser::ArrayAssignationContext *ctx) override;
    
    virtual antlrcpp::Any visitAffectArrayExpr(ifccParser::AffectArrayExprContext *ctx) override;

    virtual antlrcpp::Any visitArrayValue(ifccParser::ArrayValueContext *ctx) override;
    virtual antlrcpp::Any visitFuncall(ifccParser::FuncallContext *ctx) override;

    virtual antlrcpp::Any visitParamList(ifccParser::ParamListContext *ctx) override;

    virtual antlrcpp::Any visitParam(ifccParser::ParamContext *ctx) override;

    virtual antlrcpp::Any visitNotExpr(ifccParser::NotExprContext *ctx) override;

    string allocateTempVar();

    int getErrCount() {
        return errorCount;
    }

    vector<Error*> getErrors(){
        return errors;
    }

    int getWarningCount() {
        return warningCount;
    }

    vector<Warning*> getWarnings() {
        return warnings;
    }

    map<string, map<string, int>> getSymbolTables() {
        return symbolTables;
    }

    int getStackOffset() {
        return stackOffset;
    }

    vector<Instruction *> getInstructions() {
        return instructions;
    }

    ~Visitor() {
        for (auto it = instructions.begin(); it != instructions.end(); ++it) {
            delete *it;
        }
    }
private:
    map<string, map<string, int>> symbolTables;
    string activeSymbolTable; // function name or !global
    vector<tuple<string, int, pair<int, int>>> countUseVar; // variable name, variable nb of use, variable declaration line and position
    vector<Instruction *> instructions;
	int stackOffset = 0;
    int errorCount = 0;
    vector<Error *> errors;
    int warningCount = 0;
    vector<Warning *> warnings;

    map<string, int>& symbolTable() {
        return symbolTables.at(activeSymbolTable);
    }
};

