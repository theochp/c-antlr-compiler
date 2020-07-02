grammar ifcc;

axiom : prog       
      ;

prog : toplevel+ ;

toplevel: funcdecl;

funcdecl: 'int' NAME paramDecl bloc #intfuncdecl
		| 'void' NAME paramDecl bloc #voidfuncdecl
		;

paramDecl: '(' ('int' NAME (',' 'int' NAME)*)? ')';

bloc: '{' statement* '}';

statement: callVoid	';'		# callVoidStatement
		 | expr ';'         # exprStatement
         | declaration 	    # declStatement
		 | ifElse			# ifElseStatement
		 | whileStmnt		# whileStatement
		 | forStmnt         # forStatement
         | ret         	    # retStatement 
         ;

callVoid : NAME paramList;

whileStmnt: 'while' '(' expr ')' blocOrStatement;

forStmnt: 'for' '(' expr ';' expr ';' expr ')' blocOrStatement;

declaration: ('int'|'char') individualDeclaration? (',' individualDeclaration)* ';';

ifElse : 'if' '(' expr ')' blocOrStatement elsePart?;
elsePart: 'else' blocOrStatement;

blocOrStatement: (bloc | statement);

individualDeclaration: NAME ('=' expr)?		# valueDeclaration
	| NAME'['CONST']' arrayAssignation?		# arrayDeclaration
	| NAME'[]' arrayAssignation				# arrayDeclarationAssignation
	;

expr: NAME IN_DECREMENT  # postInDecrExpr
	| NAME paramList     # funcall
	| ADDMINUS expr 	 # unOp
	| NOT expr 	  		 # notExpr
    | IN_DECREMENT NAME  # preInDecrExpr
	| expr MULTDIV expr  # multExpr
	| expr ADDMINUS expr # addExpr
    | expr COMP_PRIO expr # compPrioExpr
	| expr COMP expr      # compExpr
	| expr BITWISE expr  # bitwiseExpr
	| NAME '=' expr		 # affectExpr
	| NAME '[' expr ']' '=' expr	# affectArrayExpr
    | '(' expr ')'	 # parExpr
	| NAME				 # nameExpr
	| CONST				 # constExpr
	| NAME '[' expr ']'				# arrayValue
	| CHAR               # charExpr
	;

arrayAssignation: '=' '{'(expr(','expr)*)? '}';

paramList : '(' (expr (',' expr)*)? ')';

ret: RETURN expr? ';';

RETURN : 'return' ;
NAME : [a-zA-Z_]+;
NOT : ('!' | '~');
MULTDIV : ('*'|'/');
ADDMINUS : ('+'|'-');
IN_DECREMENT : ('++' | '--');
COMP_PRIO : ('<='|'<'|'>='|'>');
COMP : ('=='|'!=');
BITWISE : ('&' | '|' | '^');
CONST : [0-9]+ ;
CHAR : '\'' [a-zA-Z] '\'';
COMMENT : '/*' .*? '*/' -> skip ;
SINGLECOMMENT : '//' .*? '\n' -> skip ;
DIRECTIVE : '#' .*? '\n' -> skip ;
WS    : [ \t\r\n] -> channel(HIDDEN);
