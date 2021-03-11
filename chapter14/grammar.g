Grammar lsbasi; 

expr 
	: term ((MINUS | PLUS) term ) *
	;

term
	: factor ((MUL | REAL_DIV | INT_DIV ) factor ) *
	;

factor
	: INTEGER 
	| REAL
	| RPARENT expr LPARENT
	| PLUS factor 
    | MINUS factor
	| ID
	;

assignment_statement
	: ID ASSIGN expr
	;

empty:
	;

statement
	: assignment_statement 
 	| compound_statement 
	| empty
	;

program
	: PROGRAM ID SEMI block DOT
	;

block
	: declarations compound_statement
	;

declarations
	: VAR (variable_declaration SEMI)+
	| (PROCEDURE ID LPAREN formal_parameter_list RPAREN SEMI block SEMI)*
	| empty
	;

formal_parameter_list 
    : variable_declaration
    | variable_declaration SEMI formal_parameter_list

variable_declaration
	: ID (COMMA ID)* COLON (TYPE_INTEGER | TYPE_REAL)
	;

compound_statement
	: BEGIN statement_list END
	;

statement_list
	: statement (SEMI statement)*
	;

TYPE_INTEGER:	'INTEGER'
TYPE_REAL: 		'REAL'
PROGRAM:		'PROGRAM'
VAR:			'VAR'
PROCEDURE:		'PROCEDURE'
BEGIN:			'BEGIN'
END:			'END'
MINUS:			'-'
PLUS:			'+'
MUL:			'*'
REAL_DIV:		'/'
INT_DIV:		'DIV'
RPARENT: 		'('
LPARENT: 		')'
ASSIGN:			':='
DOT 			'.'
COMMA:			','
SEMI:			';'
COLON:			':'
REAL: 			[0-9]+(.[0-9]+)?
INTEGER: 		[0-9]+
ID:				[a-zA-Z_][a-zA-Z0-9_]*
SPACES:			[ \t\n\r]+ -> skip