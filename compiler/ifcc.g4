grammar ifcc;

axiom : prog       
      ;

prog : toplevel+ ;

toplevel: funcdecl;

funcdecl: 'int' NAME paramDecl bloc;

paramDecl: LPAR ('int' NAME)? (',' 'int' NAME)* RPAR;

bloc: '{' statement* '}';

statement: expr ';'         # exprStatement
         | declaration 	    # declStatement
		 | ifElse			# ifElseStatement
         | ret         	    # retStatement
         ;


declaration: 'int' individualDeclaration? (',' individualDeclaration)* ';';

ifElse : 'if' '(' expr ')' blocOrStatement elsePart?;
elsePart: 'else' blocOrStatement;

blocOrStatement: (bloc | statement);

individualDeclaration: NAME ('=' expr)? ;

expr: NAME paramList     # funcall
	| ADDMINUS expr 	 # unOp
	| '!' expr 			 # notExpr
	| expr MULTDIV expr  # multExpr
	| expr ADDMINUS expr # addExpr
	| expr BITWISE expr  # bitwiseExpr
	| NAME '=' expr		 # affectExpr
    | LPAR expr RPAR	 # parExpr
	| NAME				 # nameExpr
	| CONST				 # constExpr
	;

paramList : LPAR expr (',' expr)* RPAR;
param : 'int' NAME;

ret: RETURN expr? ';';

RETURN : 'return' ;
NAME : [a-zA-Z_]+;
MULTDIV : ('*'|'/');
ADDMINUS : ('+'|'-');
BITWISE : ('&' | '|' | '^');
CONST : [0-9]+ ;
LPAR : '(';
RPAR : ')';
COMMENT : '/*' .*? '*/' -> skip ;
SINGLECOMMENT : '//' .*? '\n' -> skip ;
DIRECTIVE : '#' .*? '\n' -> skip ;
WS    : [ \t\r\n] -> channel(HIDDEN);
