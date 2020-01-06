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
    : modifier? FUN functionidentifier parameters (COLON type '?'?)? code
    | modifier? FUN functionidentifier parameters EQ innercode
    ;

code
    : LEFTBRACE innercodelist? RIGHTBRACE
    ;

innercodelist
    : innercode+
    ;

innercode
    :(func|expr|variable|if_|for_|while_|returned)
    ;

expr
    : identifier operator identifier (operator identifier)*
    | fcall
    | identifier singleoperator
    | assign
    | identifier
    | string operator identifier
    | identifier operator string
    | string operator string
    | returned
    ;
assign
    : identifier EQ expr
    | identifier EQ value
    ;

fcall
    : functionidentifier LEFTBRACKET fparam (COMMA fparam)* RIGHTBRACKET
    ;

functionidentifier
    : identifier
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
    : ~('\\' | '"' | '$')+
    |  ('!'|'@'|'#'|'%'|'^'|'&'|'*')
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

obj
    : identifier '.' methodcall
    | identifier '.' element
    | identifier
    ;
methodcall : fcall;
element : identifier;

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
    : LEFTBRACKET modifier? type? identifier 'in' range RIGHTBRACKET
    ;

range
    : NUM '..' NUM
    | NUM vector NUM 'step' NUM
    ;
vector
    : ('down' | 'up' | 'downTo' | 'upTo')
    ;
returned
    : RETURN identifier
    | RETURN value
    | RETURN expr
    | RETURN obj
    | RETURN NULL
    ;

parameters
    : LEFTBRACKET (parameter (COMMA parameter)*)? RIGHTBRACKET
    ;

parameter
    : identifier COLON type
    ;
    
list
    : LISTOF LEFTBRACKET string (COMMA string)* RIGHTBRACKET
    ;

value
    : NUM
    | REAL
    | string
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
    | define? identifier EQ expr
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
SEMICOLON : ';' -> skip;
NEWLINE: ('\n' | '\r\n')+ -> skip;
