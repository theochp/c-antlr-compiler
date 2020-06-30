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
  vector<const Node *> ast = visitor.visit(tree).as<vector<const Node *>>();

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
   // cout << "Test 1 de Cyrielle et Tania" << endl;
    if (visitor.getErrCount() == 0)  {
       // cout << "Test 2 de Cyrielle et Tania" << endl;
        IRGenerator irGen(ast, visitor.getSymbolTable(), visitor.getStackOffset());
       // cout << "Test 3 de Cyrielle et Tania" << endl;
        irGen.generate();
       // cout << "Test 4 de Cyrielle et Tania" << endl;
        AsmGenerator asmGen(irGen.getBlocks(), irGen.getSymbolTable());
       // cout << "Test 5 de Cyrielle et Tania" << endl;
        asmGen.generate(cout);
     //   cout << "Test 6 de Cyrielle et Tania" << endl;
        ofstream out("output.s");
   //     cout << "Test 7 de Cyrielle et Tania" << endl;
        asmGen.generate(out);
 //       cout << "Test 8 de Cyrielle et Tania" << endl;
        out.close();
 //       cout << "Test 9 de Cyrielle et Tania" << endl;
        return 0;
    }
//    cout << "Test 10 de Cyrielle et Tania" << endl;
  for (auto it = ast.begin(); it != ast.end(); ++it) {
    delete *it;
  }
  delete tree;
//    cout << "Test 12 de Cyrielle et Tania" << endl;
  return 1;
}
