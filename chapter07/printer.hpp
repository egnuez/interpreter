#include <string>
#include <cctype>
#include <stdexcept>
#include <iostream>
#include <sstream> 

namespace lsbasi {

	class Printer: public VisitorPrint {

		public:

		enum PRINT_TYPE {
			_RPN_STYLE,
			_LISP_STYLE,
			_TREE
		};

		private:

		AST *ast;
		PRINT_TYPE type;
		std::stringstream dispatcher(AST *node, int deep){
			return node->handler(this, deep);
		}

		std::stringstream print_op(BinOp *node, std::string op, int deep){
			std::stringstream ss;
			if (type == _LISP_STYLE)
				ss << "(" << op << dispatcher(node->left, deep).str() << " " << dispatcher(node->right, deep).str() << ")";
			if (type == _RPN_STYLE)
				ss << dispatcher(node->left, deep).str() << " " << dispatcher(node->right, deep).str() << op;
			if (type == _TREE){
				ss << std::string(deep, ' ');
				ss << "BinOp(" << op << ")" << std::endl;
				deep += 2;
				ss << dispatcher(node->left, deep).str();
				ss << dispatcher(node->right, deep).str();
			}
			return ss;

		}

		std::stringstream visit(Num *node, int deep){
			std::stringstream ss;
			if (type == _TREE){
				ss << std::string(deep, ' ');
				ss << "Num(" << node->value << ")" << std::endl; 
			}else
				ss << node->value;
			return ss;
		}

		std::stringstream visit(BinOp *node, int deep){
			std::stringstream ss;
			ss << print_op(node, node->op, deep).str();
			return ss;
		}

		public:
		Printer(AST * ast): ast(ast){}
		std::string print (PRINT_TYPE type){
			this->type = type;
			return dispatcher(ast, 0).str();
		}
	};
};