
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
	antlrcpp::Any visitAxiom(ifccParser::AxiomContext *ctx) override;
    
	antlrcpp::Any visitProg(ifccParser::ProgContext *ctx) override;

    antlrcpp::Any visitToplevel(ifccParser::ToplevelContext *ctx) override;

    antlrcpp::Any visitFuncdecl(ifccParser::FuncdeclContext *ctx) override;

    antlrcpp::Any visitParamDecl(ifccParser::ParamDeclContext *ctx) override;

    antlrcpp::Any visitBloc(ifccParser::BlocContext *ctx) override;

	antlrcpp::Any visitExprStatement(ifccParser::ExprStatementContext *ctx) override;

    antlrcpp::Any visitDeclStatement(ifccParser::DeclStatementContext *ctx) override;

    antlrcpp::Any visitRetStatement(ifccParser::RetStatementContext *ctx) override;

    antlrcpp::Any visitWhileStatement(ifccParser::WhileStatementContext *ctx) override;

    antlrcpp::Any visitForStatement(ifccParser::ForStatementContext *ctx) override;

    antlrcpp::Any visitDeclaration(ifccParser::DeclarationContext *ctx) override;

	antlrcpp::Any visitIndividualDeclaration(ifccParser::IndividualDeclarationContext *ctx) override;

	antlrcpp::Any visitNameExpr(ifccParser::NameExprContext *ctx) override;

    antlrcpp::Any visitConstExpr(ifccParser::ConstExprContext *ctx) override;

    antlrcpp::Any visitMultExpr(ifccParser::MultExprContext *ctx) override;

    antlrcpp::Any visitAddExpr(ifccParser::AddExprContext *ctx) override;

    antlrcpp::Any visitParExpr(ifccParser::ParExprContext *ctx) override;

    antlrcpp::Any visitBitwiseExpr(ifccParser::BitwiseExprContext *context) override;

    antlrcpp::Any visitAffectExpr(ifccParser::AffectExprContext *ctx) override;
    
    antlrcpp::Any visitCompPrioExpr(ifccParser::CompPrioExprContext *ctx) override;

    antlrcpp::Any visitCompExpr(ifccParser::CompExprContext *ctx) override;

    antlrcpp::Any visitUnOp(ifccParser::UnOpContext *ctx) override;

    antlrcpp::Any visitRet(ifccParser::RetContext *ctx) override;

    antlrcpp::Any visitFuncall(ifccParser::FuncallContext *ctx) override;

    antlrcpp::Any visitParamList(ifccParser::ParamListContext *ctx) override;

    antlrcpp::Any visitNotExpr(ifccParser::NotExprContext *ctx) override;

    antlrcpp::Any visitIfElseStatement(ifccParser::IfElseStatementContext *ctx) override;

    antlrcpp::Any visitIfElse(ifccParser::IfElseContext *ctx) override;

    antlrcpp::Any visitElsePart(ifccParser::ElsePartContext *ctx) override;

    antlrcpp::Any visitWhileStmnt(ifccParser::WhileStmntContext *ctx) override;

    antlrcpp::Any visitForStmnt(ifccParser::ForStmntContext *ctx) override;

    antlrcpp::Any visitBlocOrStatement(ifccParser::BlocOrStatementContext *ctx) override;

    int getErrCount() {
        return errorCount;
    }

    vector<Error*> getErrors(){
        return errors;
    }

    vector<Warning*> getWarnings() {
        return warnings;
    }

    map<string, map<string, int>> getSymbolTables() {
        return symbolTables;
    }

    map<string,int>& getSymbolOffsets() {
        return symbolOffsets;
    }

    ~Visitor() {
        for (auto it = instructions.begin(); it != instructions.end(); ++it) {
            delete *it;
        }
    }
private:
    map<string, map<string, int>> symbolTables;
    map<string, int> symbolOffsets;
    string activeSymbolTable; // function name or !global
    vector<tuple<string, int, pair<int, int>>> countUseVar; // variable name, variable nb of use, variable declaration line and position
    vector<Instruction *> instructions;
    int errorCount = 0;
    vector<Error *> errors;
    int warningCount = 0;
    vector<Warning *> warnings;

    map<string, int>& symbolTable() {
        return symbolTables.at(activeSymbolTable);
    }

    int incrementOffset(string func, int size) {
        int newOffset = symbolOffsets.at(func) -= size;
        symbolOffsets.emplace(func, newOffset);
        return newOffset;
    }
};

