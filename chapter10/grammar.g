Grammar lsbasi; 

expr 
	: term (('-' | '+') term ) *
	;

term
	: factor (('*' | '/' | 'DIV' ) factor ) *
	;

factor
	: '+' factor 
    | '-' factor 
	| NUM 
	| ID 
	| '(' expr ')'
	;

assignment_statement
	: ID ':=' expr
	;

empty:
	;

statement
	: assignment_statement 
 	| compound_statement 
	| empty
	;

program
	: 'PROGRAM' ID ';' block '.'
	;

block
	: declarations compound_statement
	;

declarations
	: 'VAR' (variable_declaration ';')+	
	| empty
	;

variable_declaration
	: ID (',' ID)* ':' TYPE
	;

compound_statement
	: 'BEGIN' statement_list 'END'
	;

statement_list
	: statement 
 	| statement ';' statement_list
	;

ID
	: "[a-zA-Z][a-zA-Z0-9]*"
	;

NUM
	: "[0-9]+(.[0-9]+)?"
	;

TYPE
	: "INTEGER"
	| "REAL"
	;
