grammar Kotlin;

// parser rule
prog
    : pack? import_* objlist? EOF
    ;

pack
    : PACKAGE importidentifer
    ;

import_
    : IMPORT importidentifer
    ;

objlist
    : object+
    ;

object
    : func
    | variable
    ;

func
    : modifier? FUN identifier parameters (COLON type '?'?)? code
    | modifier? FUN identifier parameters EQ innercode
    ;

code
    : LEFTBRACE innercodelist? RIGHTBRACE
    ;

innercodelist
    : innercode+
    ;

innercode
    :(func|variable|expr|if_|for_|while_|returned)
    ;

expr
    : identifier operator identifier (operator identifier)*
    | fcall
    | identifier singleoperator
    | identifier
    ;

fcall
    : identifier LEFTBRACKET fparam (COMMA fparam)* RIGHTBRACKET
    ;

fparam
    : identifier
    | expr
    | value
    ;

s_expr
    : fcall | string
    ;
string
    : DOUBLEQ (stringContent | stringExpression)* DOUBLEQ
    ;

stringContent
    : strText | strRef
    ;

stringExpression
    : DOLLAR LEFTBRACE s_expr RIGHTBRACE
    ;

strText
    : ~(BACKSLASH | DOUBLEQ | DOLLAR)+
    ;

strRef
    : DOLLAR ID
    ;

if_
    : IF condition code ifelse?
    | IF condition expr ifelse?
    ;

ifelse
    : (ELSE IF condition code)* else_
    | (ELSE IF condition expr)* else_
    ;

else_
    : ELSE code
    | ELSE expr
    ;

while_
    : WHILE condition code
    | WHILE condition expr
    ;

condition
    : LEFTBRACKET expr RIGHTBRACKET
    | LEFTBRACKET identifier 'is' type RIGHTBRACKET
    | LEFTBRACKET identifier 'is' identifier RIGHTBRACKET
    | LEFTBRACKET variable 'in' range RIGHTBRACKET
    ;

for_
    : FOR forcondition code
    | FOR forcondition expr
    ;

forcondition
    : LEFTBRACKET variable 'in' range RIGHTBRACKET
    ;

range
    : NUM '..' NUM
    | NUM ('down' | 'up') NUM 'step' NUM
    ;
returned
    : RETURN identifier
    | RETURN value
    | RETURN expr
    | RETURN NULL
    ;

parameters
    : LEFTBRACKET (parameter (COMMA parameter)*)? RIGHTBRACKET
    ;

list
    : LISTOF LEFTBRACKET string (COMMA string)* RIGHTBRACKET
    ;

value
    : NUM
    | REAL
    | string
    ;

parameter
    : identifier COLON type
    ;

importidentifer
    : (identifier|MUL) (PUNCTUATION (identifier|MUL))*
    ;

identifier
    : ID
    ;

variables
    : variable+
    ;

variable
    : define? identifier ((COLON type)? (EQ value)?)
    | define? identifier EQ list
    ;

define
    : VAR
    | VAL
    ;

type
    : DOUBLE
    | FLOAT
    | LOING
    | INT
    | SHORT
    | BYTE
    | UINT
    | ANY
    | STRING_T
    ;

modifier
    : PUBLIC
    | PRIVATE
    ;

operator
    : calcoperator
    | logicaloperator
    ;

singleoperator
    :PP | MM;

calcoperator
    :ADD|SUB|MUL|DIV;

logicaloperator
    :BIGGER|SMALLER|EQQ|NEQ;

// lexer rule

// 기호들
LEFTBRACE: '{';
RIGHTBRACE: '}';
LEFTBRACKET: '(';
RIGHTBRACKET: ')';
COLON: ':';
PUNCTUATION: '.';
DOTDOT: '..';
COMMA: ',';
QUESTIONMARK: '?';
EQ: '=';
DOUBLEQ: '"';
BACKSLASH : '\\';
DOLLAR: '$';
// operator
ADD: '+';
SUB: '-';
MUL: '*';
DIV: '/';
PP: '++';
MM: '--';

// Logical operator
BIGGER: '>';
SMALLER: '<';
NEQ: '!=';
EQQ: '==';

// modifier
PUBLIC: 'public';
PRIVATE: 'private';

// type
DOUBLE: 'Double';
FLOAT: 'Float';
LOING: 'Long';
INT: 'Int';
SHORT: 'Short';
BYTE: 'Byte';
ANY: 'Any';

STRING_T: 'String';
UINT: 'Unit';
LISTOF: 'listOf';
// define variable
VAL: 'Val' | 'val';
VAR: 'Var'|'var';

// 구분자
PACKAGE: 'package';
IMPORT: 'import';
CLASS: 'class';
FUN:'fun';
RETURN:'return';
NULL: 'null';

// control flow
IF: 'if';
ELSE: 'else';
FOR: 'for';
WHILE: 'while';

// value
ID: [A-Za-z]+[0-9]*;
NUM: [1-9][0-9]* | '0';
REAL: NUM '.' NUM;
WS: ' '+ -> skip;
NEWLINE: ('\n' | '\r\n')+ -> skip;
