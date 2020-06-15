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
#include "generator/generator.h"

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
  cout << visitor.visit(tree).as<Node*>()->print() << endl;

  if (visitor.getErrCount() == 0)  {
    /*generator gen(visitor.getInstructions(), visitor.getSymbolTable());
  
    ofstream out("output.s");
    gen.generate(cout);
    gen.generate(out);
    out.close();*/

    return 0;
  }

  return 1;
}
