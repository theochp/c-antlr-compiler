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

expr: ADDMINUS expr 	  # unOp
	| NOT expr 	  # notExpr
	| expr MULTDIV expr   # multExpr
	| expr ADDMINUS expr  # addExpr
    | expr COMP_PRIO expr # compPrioExpr
	| expr COMP expr      # compExpr
	| expr BITWISE expr   # bitwiseExpr
	| NAME '=' expr		  # affectExpr
	| '('expr')'		  # parExpr
	| NAME				  # nameExpr
	| CONST				  # constExpr
	;

ret: RETURN expr? ';';

RETURN : 'return' ;
NAME : [a-zA-Z_]+;
NOT : ('!' | '~');
MULTDIV : ('*'|'/');
ADDMINUS : ('+'|'-');
COMP_PRIO : ('<='|'<'|'>='|'>');
COMP : ('=='|'!=');
BITWISE : ('&' | '|' | '^');
CONST : [0-9]+ ;
COMMENT : '/*' .*? '*/' -> skip ;
SINGLECOMMENT : '//' .*? '\n' -> skip ;
DIRECTIVE : '#' .*? '\n' -> skip ;
WS    : [ \t\r\n] -> channel(HIDDEN);
