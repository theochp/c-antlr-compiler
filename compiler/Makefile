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

all: ifcc

ifcc: dirs antlr  main.o visitor.o ir/instruction.o generator/generator.o ast/block.o ast/constant.o ast/expression.o ast/node.o ast/operator.o ast/symbol.o 
	$(CC) $(CCARGS) $(GENERATED)/ifccBaseVisitor.cpp -o $(OUTPUT)/ifccBaseVisitor.o 
	$(CC) $(CCARGS) $(GENERATED)/ifccLexer.cpp -o $(OUTPUT)/ifccLexer.o 
	$(CC) $(CCARGS) $(GENERATED)/ifccVisitor.cpp -o $(OUTPUT)/ifccVisitor.o 
	$(CC) $(CCARGS) $(GENERATED)/ifccParser.cpp -o $(OUTPUT)/ifccParser.o 
	$(CC) $(LDARGS) $^ $(OUTPUT)/ifccBaseVisitor.o $(OUTPUT)/ifccLexer.o $(OUTPUT)/ifccVisitor.o $(OUTPUT)/ifccParser.o $(ANTLR4_LIBDIR)/libantlr4-runtime.a -o ifcc

antlr: $(GRAMMAR)
	$(ANTLR4_BINDIR)/antlr4 -visitor -no-listener -Dlanguage=Cpp -o $(GENERATED) $(GRAMMAR)

%.o : %.cpp
	$(CC) $(LDARGS) -o $@ -c $< $(CFLAGS)

dirs:
	mkdir -p $(GENERATED) 
	mkdir -p $(OUTPUT)/ir
	mkdir -p $(OUTPUT)/generator

clean:
	rm -rf $(OUTPUT)
	rm -rf $(GENERATED)
