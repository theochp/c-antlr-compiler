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

expr: expr IN_DECREMENT  # postInDecrExpr
    | ADDMINUS expr 	 # unOp
	| '!' expr 			 # notExpr
	| IN_DECREMENT expr  # preInDecrExpr
	| expr MULTDIV expr  # multExpr
	| expr ADDMINUS expr # addExpr
	| expr BITWISE expr  # bitwiseExpr
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
IN_DECREMENT : ('++' | '--');
BITWISE : ('&' | '|' | '^');
CONST : [0-9]+ ;
COMMENT : '/*' .*? '*/' -> skip ;
SINGLECOMMENT : '//' .*? '\n' -> skip ;
DIRECTIVE : '#' .*? '\n' -> skip ;
WS    : [ \t\r\n] -> channel(HIDDEN);
