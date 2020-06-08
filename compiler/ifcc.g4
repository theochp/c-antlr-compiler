grammar ifcc;

axiom : prog       
      ;

prog : 'int' 'main' '(' ')' '{' statement* '}' ;


statement: expr ';'    # exprStatement
         | declaration # declStatement
         | ret         # retStatement
         ;

declaration: 'int' NAME ('=' CONST)? ';';

expr: NAME          # varExpr
    | CONST         # constExpr
    | NAME '=' expr # affectExpr
    ;

ret: RETURN expr? ';';

RETURN : 'return' ;
NAME : [a-zA-Z_]+;
CONST : [0-9]+ ;
COMMENT : '/*' .*? '*/' -> skip ;
DIRECTIVE : '#' .*? '\n' -> skip ;
WS    : [ \t\r\n] -> channel(HIDDEN);
