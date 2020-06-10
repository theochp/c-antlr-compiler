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
  string res = visitor.visit(tree).as<string>();

  if (visitor.getErrCount() == 0)  {
    cout << res << endl;
    ofstream out("output.s");
    out << res << endl;
    out.close();
    cout << endl << endl;
    auto inst = visitor.getInstructions();
    for (auto it = inst.begin(); it != inst.end(); ++it ) {
      cout << **it << endl;
    }
    return 0;
  }

  return 1;
}
