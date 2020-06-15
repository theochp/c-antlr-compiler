grammar ifcc;

axiom : prog       
      ;

prog : 'int' 'main' '(' ')' bloc ;

bloc: '{' statement* '}';

statement: expr ';'    	# exprStatement
		 | affectation  # affectStatement
         | declaration 	# declStatement
         | ret         	# retStatement
         ;


declaration: 'int' individualDeclaration (',' individualDeclaration)* ';';

individualDeclaration: NAME ('=' CONST)? ;

affectation: NAME ('=' NAME)* '=' expr;

expr: '('expr')'		 # parExpr
	| expr MULTDIV expr  # multExpr
	| expr ADDMINUS expr # addExpr
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
