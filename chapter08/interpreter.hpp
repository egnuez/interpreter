#include <string>
#include <cctype>
#include <stdexcept>
#include <iostream>
#include <sstream> 

namespace lsbasi {

	class Interpreter: public VisitorInterpreter {

		AST *ast;
		int dispatcher(AST *node){
			return node->handler(this);
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