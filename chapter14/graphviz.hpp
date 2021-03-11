#include <string>
#include <cctype>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <fstream>

namespace lsbasi {

	class Graphviz: public VisitorGraphviz {

		private:

		AST *ast;
		std::stringstream dispatcher(AST *node, std::string parent){
			return node->handler(this, parent);
		}

		std::stringstream visit(Num *node, std::string parent){
			std::stringstream ss;
			std::stringstream actual;
			actual << "Num" << node->number;
			ss << parent << " -> " << actual.str() << ";" << std::endl;
			ss << actual.str() << " [label=\"Num(" << node->value << ")\"];" << std::endl; 
			return ss;
		}

		std::stringstream visit(Id *node, std::string parent){
			std::stringstream ss;
			std::stringstream actual;
			actual << "Id" << node->number;
			ss << parent << " -> " << actual.str() << ";" << std::endl; 
			ss << actual.str() << " [label=\"Id(" << node->value << ")\"];" << std::endl; 
			return ss;
		}

		std::stringstream visit(Assign *node, std::string parent){
			std::stringstream ss;
			std::stringstream actual;
			actual << "Assign" << node->number;
			ss << parent << " -> " << actual.str() << ";" << std::endl;
			ss << actual.str() << " [label=\"Assign\"];" << std::endl;
			ss << dispatcher(node->id, actual.str()).str();
			ss << dispatcher(node->expr, actual.str()).str();
			return ss;
		}

		std::stringstream visit(BinOp *node, std::string parent){
			std::stringstream ss;
			std::stringstream actual;
			actual << "BinOp" << node->number; 
			ss << parent << " -> " << actual.str() << ";" << std::endl;
			ss << actual.str() << " [label=\"BinOp(" << node->op << ")\"];" << std::endl;
			ss << dispatcher(node->left,  actual.str()).str();
			ss << dispatcher(node->right, actual.str()).str();
			return ss;
		}

		std::stringstream visit(UnaryOp *node, std::string parent){
			std::stringstream ss;
			std::stringstream actual;
			actual << "UnaryOp" << node->number; 
			ss << parent << " -> " << actual.str() << ";" << std::endl;
			ss << actual.str() << " [label=\"UnaryOp(" << node->op << ")\"];" << std::endl;
			ss << dispatcher(node->fact, actual.str()).str();
			return ss;
		}

		std::stringstream visit(Empty *node, std::string parent){
			std::stringstream ss;
			std::stringstream actual;
			actual << "Empty" << node->number;
			ss << parent << " -> " << actual.str() << ";" << std::endl;
			ss << actual.str() << " [label=\"Empty\"];" << std::endl;
			return ss;
		}

		std::stringstream visit(StatementList *node, std::string parent){
			std::stringstream ss;
			std::stringstream actual;
			actual << "StatementList" << node->number;
			std::vector<AST*>::iterator begin = node->statements.begin();
			std::vector<AST*>::iterator end = node->statements.end();
			ss << parent << " -> " << actual.str() << ";" << std::endl;
			ss << actual.str() << " [label=\"StatementList\"];" << std::endl;
			for (std::vector<AST*>::iterator it = begin; it != end; ++it)
				ss << dispatcher(*it, actual.str()).str();
			return ss;
		}

		std::stringstream visit(Program *node, std::string parent){
			std::stringstream ss;
			std::stringstream actual;
			actual << "Programm" << node->number;
			ss << actual.str() << "[label=\"Programm\"];" << std::endl;
			ss << dispatcher(node->id, actual.str()).str();
			ss << dispatcher(node->bloque, actual.str()).str();
			return ss;
		}

		std::stringstream visit(Block *node, std::string parent){
			std::stringstream ss;
			std::stringstream actual;
			actual << "Block" << node->number;
			ss << parent << " -> " << actual.str() << ";" << std::endl;
			ss << actual.str() << " [label=\"Block\"];" << std::endl;
			for (VarDecl * var : node->vars){
				ss << dispatcher(var, actual.str()).str();
			}
			for (ProcedureDecl * var : node->pvars){
				ss << dispatcher(var, actual.str()).str();
			}
			ss << dispatcher(node->compound_statement, actual.str()).str();
			return ss;
		}

		std::stringstream visit(Type *node, std::string parent){
			std::stringstream ss;
			std::stringstream actual;
			actual << "Type" << node->number;
			ss << parent << " -> " << actual.str() << ";" << std::endl;
			ss << actual.str() << " [label=\"Type(" << node->value << ")\"];" << std::endl;
			return ss;
		}

		std::stringstream visit(VarsDecl *node, std::string parent){
			std::stringstream ss;
			return ss;
		}

		std::stringstream visit(VarDecl *node, std::string parent){
			std::stringstream ss;
			std::stringstream actual;
			actual << "VarDecl" << node->number;
			ss << parent << " -> " << actual.str() << ";" << std::endl;
			ss << actual.str() << " [label=\"VarDecl\"];" << std::endl;
			ss << dispatcher(node->id, actual.str()).str();
			ss << dispatcher(node->type, actual.str()).str();
			return ss;
		}

		std::stringstream visit(DecList *node, std::string parent){
			std::stringstream ss;
			return ss;
		};

		std::stringstream visit(ProcedureDecl *node,  std::string parent){
			std::stringstream ss;
			std::stringstream actual;
			actual << "ProcedureDecl" << node->number;
			ss << parent << " -> " << actual.str() << ";" << std::endl;
			ss << actual.str() << " [label=\"ProcedureDecl\"];" << std::endl;
			ss << dispatcher(node->id, actual.str()).str();
			ss << dispatcher(node->block, actual.str()).str();
			return ss;
		}

		std::stringstream visit(FormalPrametersList *node,  std::string parent){
			std::stringstream ss;
			return ss;
		}

		public:
		Graphviz(AST * ast): ast(ast){}
		void graphviz (){
			std::ofstream outputFile("ast.gv");
			outputFile << "digraph G {";
			outputFile << dispatcher(ast, "").str();
			outputFile << "}";
			outputFile.close();
		}
	};
};