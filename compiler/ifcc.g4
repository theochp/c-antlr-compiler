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
         | if
         | doWhile
         ;

<<<<<<< HEAD

declaration: 'int' individualDeclaration (',' individualDeclaration)* ';';

individualDeclaration: NAME ('=' CONST)? ;

for: 'for('(individualDeclaration*|declaration|)';'(condition|)';'(instruction(,instruction)*|)')'(bloc|statement|';') ;
=======
for: 'for('(instruction|declaration|)';'(condition|)';'(instruction(','instruction)*|)')'(bloc|statement|';') ;
>>>>>>> ajout condition > et <

while: 'while('condition')'(';'|bloc|statement) ;

if: 'if('condition')' (bloc|statement) # if
	| 'if('condition')' (bloc|statement) 'else' (bloc|statement|if) # else
	;

doWhile : 'do' bloc 'while('condition');' ;

<<<<<<< HEAD
condition: intValue '==' intValue  		# egalite
	| intValue '!=' intValue 			# inegalite
	| condition '&&' condition 			# and
	| condition '||' condition 			# or
	| '('condition')' 					# parCond
	| 'true' 							# true
	| 'false' 							# false
=======
condition: intValue '==' intValue  # egalite
	| intValue '!=' intValue # inegalite
	| intValue '<' intValue # inf
	| intValue '<=' intValue # infEgal
	| intValue '>' intValue # sup
	| intValue '>=' intValue # supEgal
	| condition '&&' condition # and
	| condition '||' condition # or
	| '('condition')' #parCond
	| 'true' #true
	| 'false' #false
>>>>>>> ajout condition > et <
	;

instruction: NAME ('=' NAME)* '=' intValue # affectExpr
    ;

intValue: NAME
	| CONST
	| operande
	| fonction
	;

operande : intValue '+' intValue 	# add
	| intValue '-' intValue 		# minus
	| intValue '/' intValue 		# div
	| intValue '*' intValue 		# mult
	| '('operande')'' 				# parOpe
	;

ret: RETURN intValue? ';';

RETURN : 'return' ;
NAME : [a-zA-Z_]+;
CONST : [0-9]+ ;
COMMENT : '/*' .*? '*/' -> skip ;
DIRECTIVE : '#' .*? '\n' -> skip ;
WS    : [ \t\r\n] -> channel(HIDDEN);
