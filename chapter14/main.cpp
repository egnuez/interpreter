#include <iostream>
#include <sstream>
#include "lexer.hpp"
#include "parser-only.hpp"
// #include "printer.hpp"
// #include "graphviz.hpp"
// #include "symbols_table_builder.hpp"
// #include "interpreter.hpp"

int main (){

	try{

		/** Read Input File **/

		std::stringstream text;
		for (std::string line; std::getline(std::cin, line);) {
			text << " " << line;
		}

		/** Apply Lexer and Parser, Return AST **/

		lsbasi::Lexer lexer(text.str());
		lsbasi::ParserOnly parser(lexer);
		parser.parse();
		std::cout << "Success!" << std::endl;  

		/** Print AST **/

		// lsbasi::Printer printer(ast);
		// std::cout << printer.print() << std::endl;

		/** Print AST on Graphviz format **/

		// lsbasi::Graphviz graphviz(ast);
		// graphviz.graphviz();

		/** Semantic - Type Checking **/

		// lsbasi::SymbolsTableBuilder symbols_table_builder(ast);
		// symbols_table_builder.static_check();
		// symbols_table_builder.print_symbols();

		/** Intepret AST **/

		// lsbasi::Interpreter interpreter(ast);
		// interpreter.interpret();
		// std::cout << interpreter.environment() << std::endl;

	} catch(std::exception &e){
		std::cout << e.what() << std::endl;
	}

	return 0;
}