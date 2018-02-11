#include <string>
#include <cctype>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <vector>

namespace lsbasi {

	struct VisitorPrint {
		virtual std::stringstream visit(class Num *ast, int deep) = 0;
		virtual std::stringstream visit(class BinOp *ast, int deep) = 0;
		virtual std::stringstream visit(class UnaryOp *ast, int deep) = 0;
		virtual std::stringstream visit(class Id *ast, int deep) = 0;
		virtual std::stringstream visit(class Assign *ast, int deep) = 0;
		virtual std::stringstream visit(class Empty *ast, int deep) = 0;
		virtual std::stringstream visit(class StatementList *ast, int deep) = 0;
		virtual std::stringstream visit(class Program *ast, int deep) = 0;
		virtual std::stringstream visit(class Block *ast, int deep) = 0;
		virtual std::stringstream visit(class Type *ast, int deep) = 0;
		virtual std::stringstream visit(class VarDecl *ast, int deep) = 0;
		virtual std::stringstream visit(class DecList *ast, int deep) = 0;

	};

	struct VisitorInterpreter {
		virtual float visit(class Num *ast) = 0;
		virtual float visit(class BinOp *ast) = 0;
		virtual float visit(class UnaryOp *ast) = 0;
		virtual float visit(class Id *ast) = 0;
		virtual float visit(class Assign *ast) = 0;
		virtual float visit(class Empty *ast) = 0;
		virtual float visit(class StatementList *ast) = 0;
		virtual float visit(class Program *ast) = 0;
		virtual float visit(class Block *ast) = 0;
		virtual float visit(class Type *ast) = 0;
		virtual float visit(class VarDecl *ast) = 0;
		virtual float visit(class DecList *ast) = 0;
	};

	struct AST {
		virtual std::stringstream handler(VisitorPrint * v, int deep) = 0;
		virtual float handler(VisitorInterpreter * v) = 0;
	};

	struct Num: public AST {
		Token * token;
		float value;
		Num(Token * token):token(token){
			value = std::stoi(token->value);
		}
		std::stringstream handler(VisitorPrint * v, int deep){
			return v->visit(this, deep);
		}
		float handler(VisitorInterpreter * v){
			return v->visit(this);
		}
	};

	struct Id: public AST {
		Token * token;
		std::string value;
		Id(Token * token):token(token),value(token->value){}
		std::stringstream handler(VisitorPrint * v, int deep){
			return v->visit(this, deep);
		}
		float handler(VisitorInterpreter * v){
			return v->visit(this);
		}
	};

	struct BinOp: public AST {
		AST * left;
		Token * token;
		std::string op;
		AST * right;
		BinOp(Token * token, AST * left, AST * right): token(token), left(left), right(right), op(token->value){}
		std::stringstream handler(VisitorPrint * v, int deep){
			return v->visit(this, deep);
		}
		float handler(VisitorInterpreter * v){
			return v->visit(this);
		}
	};

	struct UnaryOp: public AST {
		AST * fact;
		Token * token;
		std::string op;
		UnaryOp(Token * token, AST * fact): token(token), fact(fact), op(token->value){}
		std::stringstream handler(VisitorPrint * v, int deep){
			return v->visit(this, deep);
		}
		float handler(VisitorInterpreter * v){
			return v->visit(this);
		}
	};

	struct Assign: public AST {
		AST * id;
		AST * expr;
		Assign(AST * id, AST * expr): id(id), expr(expr){}
		std::stringstream handler(VisitorPrint * v, int deep){
			return v->visit(this, deep);
		}
		float handler(VisitorInterpreter * v){
			return v->visit(this);
		}
	};

	struct Empty: public AST {
		Empty(){};
		std::stringstream handler(VisitorPrint * v, int deep){
			return v->visit(this, deep);
		}
		float handler(VisitorInterpreter * v){
			return v->visit(this);
		}
	};

	struct StatementList: public AST {
		std::vector<AST *> statements;
		StatementList(std::vector<AST *> statements): statements(statements){};
		std::stringstream handler(VisitorPrint * v, int deep){
			return v->visit(this, deep);
		}
		float handler(VisitorInterpreter * v){
			return v->visit(this);
		}
	};

	struct Program: public AST {
		AST * id;
		AST * bloque;
		Program(AST * id, AST * bloque): id(id), bloque(bloque){}
		std::stringstream handler(VisitorPrint * v, int deep){
			return v->visit(this, deep);
		}
		float handler(VisitorInterpreter * v){
			return v->visit(this);
		}
	};

	struct Block: public AST {
		AST * declarations;
		AST * compound_statement;
		Block(AST * declarations, AST * compound_statement): 
			declarations(declarations), 
			compound_statement(compound_statement){}
		std::stringstream handler(VisitorPrint * v, int deep){
			return v->visit(this, deep);
		}
		float handler(VisitorInterpreter * v){
			return v->visit(this);
		}
	};

	struct DecList: public AST {
		std::vector<AST *> declarations;
		DecList(std::vector<AST *> declarations): declarations(declarations){};
		std::stringstream handler(VisitorPrint * v, int deep){
			return v->visit(this, deep);
		}
		float handler(VisitorInterpreter * v){
			return v->visit(this);
		}
	};

	struct VarDecl: public AST {
		std::vector<AST *> ids;
		AST * type;
		VarDecl(std::vector<AST *> ids, AST * type): ids(ids), type(type){}
		std::stringstream handler(VisitorPrint * v, int deep){
			return v->visit(this, deep);
		}
		float handler(VisitorInterpreter * v){
			return v->visit(this);
		}
	};

	struct Type: public AST {
		Token * token;
		std::string value;
		Type(Token * token): token(token), value(token->value){}
		std::stringstream handler(VisitorPrint * v, int deep){
			return v->visit(this, deep);
		}
		float handler(VisitorInterpreter * v){
			return v->visit(this);
		}
	};

	class Parser {

		Token * current_token;
		Lexer lexer;

		int error (std::string description) {
			throw std::runtime_error(description);
		}

		void eat (Token::TokenType type){
			if (current_token->type == type)
				current_token = lexer.get_next_token();
			else
				error("Error 0 => " + current_token->value);
		}

		AST * factor(){
			/*
			factor
				: INTEGER 
				| REAL
				| RPARENT expr LPARNET
				| PLUS factor 
    			| MINUS factor
				| ID
				;
			*/
			AST * ast;

			Token * tk;
			tk = current_token;

			switch(current_token->type){
				case Token::_INTEGER:
					ast = new Num(current_token);
					eat(Token::_INTEGER);
					break;
				case Token::_REAL:
					ast = new Num(current_token);
					eat(Token::_REAL);
					break;
				case Token::_LPAREN:
					eat(Token::_LPAREN);
					ast = expr();
					eat(Token::_RPAREN);
					break;
				case Token::_PLUS:
					eat(Token::_PLUS);
					ast = new UnaryOp(tk, factor());
					break;
				case Token::_MINUS:
					eat(Token::_MINUS);
					ast = new UnaryOp(tk, factor());
					break;
				default:
					ast = new Id(tk);
					eat(Token::_ID);
					break;
			}

			return ast;
		}

		AST * term(){
			/*
			term
				: factor ((MUL | REAL_DIV | INT_DIV ) factor ) *
				;
			*/
			AST * ast = factor();

			while((current_token->type == Token::_MUL) 
			|| (current_token->type == Token::_INT_DIV)
			|| (current_token->type == Token::_REAL_DIV)){
					
				Token * tk;
				tk = current_token;

				if (current_token->type == Token::_MUL)
					eat(Token::_MUL);
				else if (current_token->type == Token::_INT_DIV)
					eat(Token::_INT_DIV);
				else if (current_token->type == Token::_REAL_DIV)
					eat(Token::_REAL_DIV);

				ast = new BinOp(tk, ast, factor());
			}

			return ast;
		}

		AST * expr(){
			/*
			expr 
				: term ((MINUS | PLUS) term ) *
				;
			
			*/
			AST * ast = term();

			while((current_token->type == Token::_PLUS) || (current_token->type == Token::_MINUS)){

				Token * tk;
				tk = current_token;

				if (current_token->type == Token::_PLUS)
					eat(Token::_PLUS);

                else if (current_token->type == Token::_MINUS)
					eat(Token::_MINUS);

				ast = new BinOp(tk, ast, term());

			}

			return ast;
		}

		AST * assignment_statement(){
			/*
			assignment_statement
				: ID SEMI expr
				;
			*/
			AST * var = new Id(current_token);
			eat(Token::_ID);
			eat(Token::_ASSIGN);
			AST * ast = new Assign(var, expr());
			return ast;
		}

		AST * empty(){
			/*
			empty
				: -
				;
			*/
			AST * ast = new Empty();
			return ast;
		}

		AST * statement(){
			/*
			statement
				: assignment_statement 
 				| compound_statement 
				| empty
				;
			*/
			AST * ast;

			if(current_token->type == Token::_ID){
				ast = assignment_statement();
			}else if(current_token->type == Token::_BEGIN){
				ast = compound_statement();
			}else{
				ast = empty();
			}
			return ast;
		}

		AST * statement_list(){
			/*
			statement_list
				: statement 
 				| statement SEMI statement_list
				;
			*/
			AST * ast; 
			std::vector<AST *> statements;
			statements.push_back(statement());
			while(current_token->type == Token::_SEMI){
				eat(Token::_SEMI);
				statements.push_back(statement());
			}
			return new StatementList(statements); 
		}

		AST * compound_statement(){
			/*
			compound_statement
				: BEGIN statement_list END
				;
			*/
			AST * ast;
			eat(Token::_BEGIN);
			ast = statement_list();
			eat(Token::_END);
			return ast;
		}

		AST * variable_declaration(){
			/*
			variable_declaration
				: ID (COMMA ID)* COLON TYPE
				;
			*/
			std::vector<AST *> variables;
			variables.push_back(new Id(current_token));
			eat(Token::_ID);
			while(current_token->type == Token::_COMMA){
				eat(Token::_COMMA);
				variables.push_back(new Id(current_token));
				eat(Token::_ID);
			}
			eat(Token::_COLON);
			Token * tk;
			tk = current_token;
			if ((current_token->type == Token::_TYPE_INTEGER) || 
				(current_token->type == Token::_TYPE_REAL));
			eat(current_token->type); 
			return new VarDecl(variables, new Type(tk));

		}

		AST * declarations(){
			/*
			declarations
				: VAR (variable_declaration SEMI)+	
				| empty
				;
			*/
			std::vector<AST *> decs;
			if(current_token->type == Token::_VAR){
				eat(Token::_VAR);
				decs.push_back(variable_declaration());
				eat(Token::_SEMI);
				while (current_token->type == Token::_ID){
					decs.push_back(variable_declaration());
					eat(Token::_SEMI);
				}
				return new DecList(decs);
			}else{
				return empty();
			}
		}

		AST * block(){
			/*
			block
				: declarations compound_statement
				;
			*/
			return new Block(declarations(), compound_statement());
		}

		AST * program(){
			/*
			program
				: PROGRAM ID SEMI block DOT
				;
			*/
			eat(Token::_PROGRAM);
			AST * var = new Id(current_token);
			eat(Token::_ID);
			eat(Token::_SEMI);
			AST * blk = block();
			eat(Token::_DOT);
			return new Program(var, blk);
		}

		public:

		Parser(lsbasi::Lexer& lexer): lexer(lexer) {
			current_token = this->lexer.get_next_token();
		}

		AST * parse(){
			AST * ast = program();
			if (current_token->type != Token::_EOF)
				error("Error 2");
			return ast;
		}

	};
};
