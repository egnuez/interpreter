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
	| empty
	;

variable_declaration
	: ID (COMMA ID)* COLON TYPE
	;

compound_statement
	: BEGIN statement_list END
	;

statement_list
	: statement 
 	| statement SEMI statement_list
	;

TYPE_INTEGER:	'INTEGER'
TYPE_REAL: 		'REAL'
PROGRAM:		'PROGRAM'
VAR:			'VAR'
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