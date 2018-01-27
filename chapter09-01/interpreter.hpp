#include <string>
#include <cctype>
#include <stdexcept>
#include <iostream>
#include <sstream> 
#include <map>

namespace lsbasi {

	class Interpreter: public VisitorInterpreter {

		AST *ast;
		std::map<std::string, int> symbols;
		int dispatcher(AST *node){
			return node->handler(this);
		}

		int visit(Assign * node){
			std::string var = (static_cast<Id *>(node->id))->value;
			int value = dispatcher(node->expr);
			symbols[var] = value;
			return value;
		}

		int visit(Id * node){
			std::string var = node->value;
			if (symbols.count(var))
				return symbols[var];
			else
				throw std::runtime_error("Invalid Variable");

		}

		int visit(Num *node){
			return node->value;
		}

		int visit(BinOp *node){
			int ret = 0;

			switch(node->token->type){
				case Token::_PLUS:
					ret = dispatcher(node->left) + dispatcher(node->right);
					break;
				case Token::_MINUS:
					ret = dispatcher(node->left) - dispatcher(node->right);
					break;
				case Token::_MUL:
					ret = dispatcher(node->left) * dispatcher(node->right);
					break;
				case Token::_DIV:
					ret = dispatcher(node->left) / dispatcher(node->right);
					break;
			}
			return ret;
		}

		int visit(UnaryOp *node){
			int ret = 0;
			switch(node->token->type){
				case Token::_PLUS:
					ret = (+1) * dispatcher(node->fact);
					break;
				case Token::_MINUS:
					ret = (-1) * dispatcher(node->fact);
				break;
			}
			return ret;
		}

		public:
		Interpreter(AST * ast): ast(ast){}
		int interpret (){
			return dispatcher(ast);
		}
	};
};