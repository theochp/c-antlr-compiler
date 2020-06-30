OUTPUT=output
GENERATED=antlr4-generated
GRAMMAR=ifcc.g4

#override with your local installation
ANTLR4_BINDIR=/shares/public/tp/ANTLR4-CPP/bin
ANTLR4_INCDIR=/shares/public/tp/ANTLR4-CPP/antlr4-runtime
ANTLR4_LIBDIR=/shares/public/tp/ANTLR4-CPP/lib

CC=clang++
CCARGS=-g -c -I $(ANTLR4_INCDIR) -I $(GENERATED) -std=c++11 -Wno-defaulted-function-deleted -Wno-unknown-warning-option
LDARGS=-g

FILTER=dirs antlr

all: ifcc

ifcc: dirs antlr main.o ast/node.o visitor.o ir/irfunc.o ir/instruction.o ir/irblock.o generator/asm_generator.o generator/ir_generator.o ast/block.o ast/constant.o ast/expression.o ast/operator.o ast/variable.o ast/assignement.o ast/char.o ast/statement.o ast/return.o ast/declaration.o ast/unexpression.o ast/unoperator.o ast/func.o ast/arrayValue.o ast/arrayDeclaration.o ast/logicalNot.o ast/funccall.o ast/funcparam.o ast/char.o ast/ifelse.o ast/while.o ast/for.o static-analysis/warning.o static-analysis/error.o static-analysis/doubleDeclaration.o static-analysis/undeclaredVariable.o static-analysis/unusedVariable.o
	$(CC) $(CCARGS) $(GENERATED)/ifccBaseVisitor.cpp -o $(OUTPUT)/ifccBaseVisitor.o 
	$(CC) $(CCARGS) $(GENERATED)/ifccLexer.cpp -o $(OUTPUT)/ifccLexer.o 
	$(CC) $(CCARGS) $(GENERATED)/ifccVisitor.cpp -o $(OUTPUT)/ifccVisitor.o
	$(CC) $(CCARGS) $(GENERATED)/ifccParser.cpp -o $(OUTPUT)/ifccParser.o 
	$(CC) $(LDARGS) $(addprefix $(OUTPUT)/,$(filter-out $(FILTER),$^)) $(OUTPUT)/ifccBaseVisitor.o $(OUTPUT)/ifccLexer.o $(OUTPUT)/ifccVisitor.o $(OUTPUT)/ifccParser.o $(ANTLR4_LIBDIR)/libantlr4-runtime.a -o ifcc

antlr: $(GRAMMAR)
	$(ANTLR4_BINDIR)/antlr4 -visitor -no-listener -Dlanguage=Cpp -o $(GENERATED) $(GRAMMAR)

%.o : %.cpp
	$(CC) $(CCARGS) $< -o $(OUTPUT)/$@

dirs:
	mkdir -p $(GENERATED)
	mkdir -p $(OUTPUT)/ir
	mkdir -p $(OUTPUT)/generator
	mkdir -p $(OUTPUT)/ast
	mkdir -p $(OUTPUT)/static-analysis

clean:
	rm -rf $(OUTPUT)
	rm -rf $(GENERATED)