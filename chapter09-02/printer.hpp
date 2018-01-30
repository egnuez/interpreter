#include <string>
#include <cctype>
#include <stdexcept>
#include <iostream>
#include <sstream> 

namespace lsbasi {

	class Printer: public VisitorPrint {

		private:

		AST *ast;
		std::stringstream dispatcher(AST *node, int deep){
			return node->handler(this, deep);
		}

		std::stringstream print_op(BinOp *node, std::string op, int deep){
			std::stringstream ss;
			ss << std::string(deep, ' ');
			ss << "BinOp(" << op << ")" << std::endl;
			deep += 2;
			ss << dispatcher(node->left, deep).str();
			ss << dispatcher(node->right, deep).str();
			return ss;

		}

		std::stringstream visit(Num *node, int deep){
			std::stringstream ss;
			ss << std::string(deep, ' ');
			ss << "Num(" << node->value << ")" << std::endl; 
			return ss;
		}

		std::stringstream visit(Id *node, int deep){
			std::stringstream ss;
			ss << std::string(deep, ' ');
			ss << "Id(" << node->value << ")" << std::endl; 
			return ss;
		}

		std::stringstream visit(Assign *node, int deep){
			std::stringstream ss;
			ss << std::string(deep, ' ');
			ss << "Assignment()" << std::endl;
			deep += 2;
			ss << dispatcher(node->id, deep).str();
			ss << dispatcher(node->expr, deep).str();
			return ss;
		}

		std::stringstream visit(BinOp *node, int deep){
			std::stringstream ss;
			ss << print_op(node, node->op, deep).str();
			return ss;
		}

		std::stringstream visit(UnaryOp *node, int deep){
			std::stringstream ss;
			ss << std::string(deep, ' ');
			ss << "UnaryOp(" << node->op << ")" << std::endl;
			ss << dispatcher(node->fact, deep).str();
			return ss;
		}

		std::stringstream visit(Empty *node, int deep){
			std::stringstream ss;
			return ss;
		}

		std::stringstream visit(StatementList *node, int deep){
			std::stringstream ss;
			std::vector<AST*>::iterator begin = node->statements.begin();
			std::vector<AST*>::iterator end = node->statements.end();
			for (std::vector<AST*>::iterator it = begin; it != end; ++it)
				ss << dispatcher(*it, deep).str();
			return ss;
		}

		public:
		Printer(AST * ast): ast(ast){}
		std::string print (){
			return dispatcher(ast, 0).str();
		}
	};
};