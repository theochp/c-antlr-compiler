#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <time.h>

#include "antlr4-runtime.h"
#include "antlr4-generated/ifccLexer.h"
#include "antlr4-generated/ifccParser.h"
#include "antlr4-generated/ifccBaseVisitor.h"
#include "visitor.h"
#include "generator/ir_generator.h"
#include "generator/asm_generator.h"

using namespace antlr4;
using namespace std;


int main(int argn, const char **argv) {
  srand(time(NULL));
  stringstream in;
  if (argn==2) {
     ifstream lecture(argv[1]);
     in << lecture.rdbuf();
  }
  ANTLRInputStream input(in.str());
  ifccLexer lexer(&input);
  CommonTokenStream tokens(&lexer);

  tokens.fill();
  //for (auto token : tokens.getTokens()) {
  //  std::cout << token->toString() << std::endl;
  //}


  ifccParser parser(&tokens);

  tree::ParseTree* tree = parser.axiom();

	if (0 != parser.getNumberOfSyntaxErrors()) {
		cerr << "Syntax error, aborting compilation" << endl;
		return 1;
	}
	
  Visitor visitor;
  Node *ast = visitor.visit(tree).as<Node*>();

    if (visitor.getErrors().size() > 0){
        cout <<  "# " << to_string(visitor.getErrors().size()) << " error(s)" << endl;
    }

    for (int i=0; i<visitor.getErrors().size(); i++){
        cout << visitor.getErrors().at(i)->getMessage() << endl;
    }

    if (visitor.getWarnings().size() > 0){
        cout << "# " << to_string(visitor.getWarnings().size()) << " warning(s)" << endl;
    }

    for (int i=0; i<visitor.getWarnings().size(); i++){
        cout << visitor.getWarnings().at(i)->getMessage() << endl;
    }

    if (visitor.getErrCount() == 0)  {
        IRGenerator irGen(ast, visitor.getSymbolTable(), visitor.getStackOffset());
        irGen.generate();
        AsmGenerator asmGen(irGen.getInstructions(), irGen.getSymbolTable());

        asmGen.generate(cout);

        ofstream out("output.s");
        asmGen.generate(out);
        out.close();

        return 0;
    }

  //delete ast;

  return 1;
}
