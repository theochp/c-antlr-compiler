grammar ifcc;

axiom : prog       
      ;

prog : 'int' 'main' '(' ')' bloc ;

bloc: '{' statement* '}';

statement: instruction ';'    # instructionStatement
         | declaration # declStatement
         | ret         # retStatement
         ;

declaration: 'int' NAME ('=' CONST)? ';';

instruction: NAME ('=' NAME)* '=' intValue # affectExpr
    ;

intValue: NAME
	| CONST
	| operande
	| fonction
	;

operande : intValue '+' intValue
	| intValue '-' intValue
	| intValue '/' intValue
	| intValue '*' intValue
	;


ret: RETURN intValue? ';';

RETURN : 'return' ;
NAME : [a-zA-Z_]+;
CONST : [0-9]+ ;
COMMENT : '/*' .*? '*/' -> skip ;
DIRECTIVE : '#' .*? '\n' -> skip ;
WS    : [ \t\r\n] -> channel(HIDDEN);
