grammar ifcc;

axiom : prog       
      ;

prog : 'int' 'main' '(' ')' bloc ;

bloc: '{' statement* '}';

statement: instruction ';'    # instructionStatement
         | declaration # declStatement
         | ret         # retStatement
         | for
         | while
         | ifElse
         | doWhile
         ;

for: 'for('(instruction|declaration|)';'(condition|)';'(instruction(,instruction)*|)')'(bloc|statement|';') ;

while: 'while('condition')'(';'|bloc|statement) ;

ifElse: 'if('condition')' (bloc|statement) # if
	| 'if('condition')' (bloc|statement) 'else' (bloc|statement) # else
	;

else: (bloc|statement) ;

doWhile : 'do' bloc 'while('condition');' ;

condition: intValue '==' intValue  # egalite
	| intValue '!=' intValue #inegalite
	| condition '&&' condition # and
	| condition '||' condition # or
	| '('condition')' #parCond
	| 'true' #true
	| 'false' #false
	;


declaration: 'int' NAME ('=' CONST)? ';';

instruction: NAME ('=' NAME)* '=' intValue # affectExpr
    ;

intValue: NAME
	| CONST
	| operande
	| fonction
	;

operande : intValue '+' intValue # add
	| intValue '-' intValue # minus
	| intValue '/' intValue # div
	| intValue '*' intValue # mult
	| '('operande')'' # parOpe
	;

ret: RETURN intValue? ';';

RETURN : 'return' ;
NAME : [a-zA-Z_]+;
CONST : [0-9]+ ;
COMMENT : '/*' .*? '*/' -> skip ;
DIRECTIVE : '#' .*? '\n' -> skip ;
WS    : [ \t\r\n] -> channel(HIDDEN);
