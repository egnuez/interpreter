#include <string>
#include <cctype>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <map>

namespace lsbasi {

	class Symbol {
		public:
			std::string name;
			Symbol(std::string name):name(name){}
			virtual std::string repr() = 0;
		
	};

	class BuildInType : public Symbol {
		public:
			BuildInType(std::string name):Symbol(name){}
			std::string  repr(){
				std::stringstream ss;
				ss << "<" << name << ">";
				return ss.str();
			}
	};

	class VarType : public Symbol {
		public:
			BuildInType * type;
			VarType(std::string name, BuildInType * type):Symbol(name), type(type){}
			std::string repr(){
				std::stringstream ss;
				ss << "<" << name << ":" << type->repr() << ">";
				return ss.str();
			}
	};

	class ProcedureType : public Symbol {
		public:
			BuildInType * type;
			ProcedureType(std::string name):Symbol(name) {}
			std::string repr(){
				std::stringstream ss;
				ss << "<" << name << ":" << "[Procedure]" << ">";
				return ss.str();
			}
	};

	class SymbolsTable {
		
		public:
		
			std::map<std::string, Symbol*> symbols;
			SymbolsTable(){
				define(new BuildInType("INTEGER"));
				define(new BuildInType("REAL"));
			}
			
			void define(Symbol * symbol){
				symbols[symbol->name] = symbol;
			}

			Symbol * lookup(std::string name){
				return symbols[name];
			}

			int count(std::string name){
				return symbols.count(name);
			}

			void print(){
				std::cout << "--- Symbols Table ---" << std::endl;
				for (std::map<std::string, Symbol*>::iterator it = symbols.begin(); it != symbols.end(); ++it){
					std::cout << it->first << " - " << it->second->repr() << std::endl;
				}
				std::cout << "---------------------" << std::endl;
			}
	};

	class SymbolsTableBuilder: public VisitorSymbolsTable {

		public:

		AST *ast;
		SymbolsTable symbols;
		
		void dispatcher(AST *node){
			node->handler(this);
		}

		void visit(Num *node){
		}

		void visit(Id *node){
			if (!symbols.count(node->value)){
				std::stringstream error;
				error << "[1]Undefined variable => " << node->value;
				throw std::runtime_error(error.str());
			}
		}

		void visit(Assign *node){
			std::string var = (static_cast<Id *>(node->id))->value;
			if (!symbols.count(var)){
				std::stringstream error;
				error << "[2]Undefined variable => " << var;
				throw std::runtime_error(error.str());
			}
			dispatcher(node->expr);
		}

		void visit(BinOp *node){
			dispatcher(node->left);
			dispatcher(node->right);
		}

		void visit(UnaryOp *node){
			dispatcher(node->fact);
		}

		void visit(Empty *node){
		}

		void visit(StatementList *node){

			std::vector<AST*>::iterator begin = node->statements.begin();
			std::vector<AST*>::iterator end = node->statements.end();
			for (std::vector<AST*>::iterator it = begin; it != end; ++it)
				dispatcher(*it);

		}

		void visit(Program *node){
			dispatcher(node->bloque);
		}

		void visit(Block *node){
			for (VarDecl * var : node->vars)
				dispatcher(var);
			for (ProcedureDecl * var : node->pvars)
				dispatcher(var);
			dispatcher(node->compound_statement);
		}

		void visit(Type *node){
		}

		void visit(VarsDecl *node){
		}

		void visit(VarDecl *node){
			std::string var = (static_cast<Id *>(node->id))->value;
			std::string type = (static_cast<Type *>(node->type))->value;
			symbols.define(new VarType(var, new BuildInType(type)));
		}

		void visit(DecList *node){
		};

		void visit(ProcedureDecl *node){
			std::string var = (static_cast<Id *>(node->id))->value;
			symbols.define(new ProcedureType(var));
			dispatcher(node->block);
		}

		void visit(FormalPrametersList *node){
		}

		public:
		
		SymbolsTableBuilder(AST * ast): ast(ast){}

		bool static_check (){
			dispatcher(ast);
			return true;
		}

		void print_symbols() {
			symbols.print();
		}
	};

};