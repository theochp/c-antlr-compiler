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


declaration: 'int' individualDeclaration (',' individualDeclaration)* ';';

individualDeclaration: NAME ('=' expr)? ;

expr: ADDMINUS expr 	 # unOp
	| expr MULTDIV expr  # multExpr
	| expr ADDMINUS expr # addExpr
	| NAME '=' expr		 # affectExpr
	| '('expr')'		 # parExpr
	| NAME				 # nameExpr
	| CONST				 # constExpr
	;

ret: RETURN expr? ';';

RETURN : 'return' ;
NAME : [a-zA-Z_]+;
MULTDIV : ('*'|'/');
ADDMINUS : ('+'|'-');
CONST : [0-9]+ ;
COMMENT : '/*' .*? '*/' -> skip ;
DIRECTIVE : '#' .*? '\n' -> skip ;
WS    : [ \t\r\n] -> channel(HIDDEN);
