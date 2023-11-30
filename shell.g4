grammar shell;
/*Analizador sintáctico*/
prog: stat+ ;

stat
    : expr NEWLINE          # printExpr //Expresión seguido de un salto de línea
    | ID '=' expr NEWLINE   # assign //Una asignación
    | IF comp THEN NEWLINE stat+ (ELSE stat+)? ENDIF NEWLINE # ifStatement //Estructura condicional IF-ELSE
    | NEWLINE               # blank // Un salto de línea o espacio en blanco
    ;

expr
    : MUL expr expr   # Mul //Operación de multiplicación
    | DIV expr expr   # Div
    | ADD expr expr   # Add
    | SUB expr expr   # Sub
    | NUMBER          # Number
    | ID              # Id //El identificador
    ;
comp
    : op=('<' | '>' | '==' | '!=') expr expr  # Comparison // Comparación
    ;

/*Analizador léxico*/
IF: 'if' ;
THEN: 'then';
ELSE: 'else' ;
ENDIF: 'endif' ;

MUL: '*' ;
DIV: '/' ;
ADD: '+' ;
SUB: '-' ;


ID: [a-zA-Z]+ ;
NUMBER: [+-]?([0-9]*[.])?[0-9]+ ; //Representa un número, puede ser positivo o negativo, con o sin parte decimal
NEWLINE: '\r'? '\n' ; //Define un salto de línea
COMMENT: [#].*? NEWLINE -> skip ; //Ignora los comentarios
WS: [ \t]+ -> skip ; //Ignora los espacios en blanco
