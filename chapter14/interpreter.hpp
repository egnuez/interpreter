#include <string>
#include <cctype>
#include <stdexcept>
#include <iostream>
#include <sstream> 
#include <map>

namespace lsbasi {

	class Interpreter: public VisitorInterpreter {

		AST *ast;
		std::map<std::string, float> symbols;
		float dispatcher(AST *node){
			return node->handler(this);
		}

		float visit(Assign * node){
			std::string var = (static_cast<Id *>(node->id))->value;
			float value = dispatcher(node->expr);
			symbols[var] = value;
			return value;
		}

		float visit(Id * node){
			std::stringstream error;
			std::string var = node->value;
			if (symbols.count(var))
				return symbols[var];
			else{
				error << "Undefined variable => " << var;
				throw std::runtime_error(error.str());
			}
		}

		float visit(Num *node){
			return node->value;
		}

		float visit(BinOp *node){
			float ret = 0;

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
				case Token::_INT_DIV:
					ret = int(dispatcher(node->left) / dispatcher(node->right));
					break;
				case Token::_REAL_DIV:
					ret = dispatcher(node->left) / dispatcher(node->right);
					break;
			}
			return ret;
		}

		float visit(UnaryOp *node){
			float ret = 0;
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

		float visit(Empty *node){
			return 0;
		}

		float visit(StatementList *node){
			float ret = 0;
			std::vector<AST*>::iterator begin = node->statements.begin();
			std::vector<AST*>::iterator end = node->statements.end();
			for (std::vector<AST*>::iterator it = begin; it != end; ++it)
				ret = dispatcher(*it);
			return ret;
		}

		float visit(Program *node){
			float ret = 0;
			//dispatcher(node->id);
			dispatcher(node->bloque);
			return ret;
		}

		float visit(Block *node){
			float ret = 0;
			for (VarDecl * var : node->vars){
				dispatcher(var);
			}
			dispatcher(node->compound_statement);
			return ret;
		}

		float visit(DecList *node){
			float ret = 0;
			return ret;
		}

		float visit(Type *node){
			float ret = 0;
			return ret;
		}

		float visit(VarsDecl *node){
			float ret = 0;
			return ret;
		}

		float visit(VarDecl *node){
			return 0.0;
		}

		float visit(ProcedureDecl *ast){
			return 0.0;
		}

		float visit(FormalPrametersList *ast){
			return 0.0;
		}

		public:

		std::string environment(){
			std::stringstream ss;
			std::string sep = "";
			ss << "{";
  			for (std::map<std::string, float>::iterator it=symbols.begin(); it!=symbols.end(); ++it){
				ss << sep << it->first << " => " << it->second;
				sep = ",";
			}
			ss << "}";
			return ss.str();
		}

		Interpreter(AST * ast): ast(ast){}
		float interpret (){
			return dispatcher(ast);
		}
	};
};