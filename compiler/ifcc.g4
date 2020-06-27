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
         | ret         	    # retStatement
         ;


declaration: 'int' individualDeclaration? (',' individualDeclaration)* ';';

individualDeclaration: NAME ('=' expr)? # valueDeclaration
	| NAME'['CONST']' arrayAssignation? # arrayDeclaration
	| NAME'[]' arrayAssignation 		# arrayDeclarationAssignation
	;

arrayAssignation: '=' '{'(CONST(','CONST)*)? '}';

expr: NAME paramList     			# funcall
    | ADDMINUS expr 	  			# unOp
	| NOT expr 	  					# notExpr
	| expr MULTDIV expr   			# multExpr
	| expr ADDMINUS expr  			# addExpr
    | expr COMP_PRIO expr			# compPrioExpr
	| expr COMP expr      			# compExpr
	| expr BITWISE expr  			# bitwiseExpr
	| NAME '=' expr		 			# affectExpr
    | LPAR expr RPAR	 			# parExpr
	| NAME				 			# nameExpr
	| CONST				 			# constExpr
	| NAME '[' expr ']'				# arrayValue
	| NAME '[' expr ']' '=' expr 	# affectArrayExpr
	;

paramList : LPAR expr (',' expr)* RPAR;
param : 'int' NAME;

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
LPAR : '(';
RPAR : ')';
COMMENT : '/*' .*? '*/' -> skip ;
SINGLECOMMENT : '//' .*? '\n' -> skip ;
DIRECTIVE : '#' .*? '\n' -> skip ;
WS    : [ \t\r\n] -> channel(HIDDEN);
