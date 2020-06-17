grammar ifcc;

axiom : prog       
      ;

prog : toplevel+ ;

toplevel: funcdecl;

funcdecl: 'int' NAME '()' bloc;

bloc: '{' statement* '}';

statement: expr ';'         # exprStatement
         | declaration 	    # declStatement
         | ret         	    # retStatement
         ;


declaration: 'int' individualDeclaration? (',' individualDeclaration)* ';';

individualDeclaration: NAME ('=' expr)? ;

expr: NAME paramList     # funcall
	| ADDMINUS expr 	 # unOp
	| '!' expr 			 # notExpr
	| ADDMINUS expr 	 # unOp
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
