#include <iostream>
#include "lexer.hpp"
#include "parser.hpp"
#include "printer.hpp"
#include "interpreter.hpp"

int main (){

	while(true){

		/** Read Input Expression **/
		
		std::string text;
		std::cout << "ctrl+c to exit" << std::endl;
		std::cout << "calc> ";
		std::cin >> text;

		/** Apply Lexer and Parser, Return AST **/
		
		lsbasi::Lexer lexer(text);
		lsbasi::Parser parser(lexer);
		lsbasi::AST *ast = parser.expr();
		
		/** Print AST in three different formats **/
		
		lsbasi::Printer printer(ast);
//		std::cout << printer.print(lsbasi::Printer::_RPN_STYLE) << std::endl;
//		std::cout << printer.print(lsbasi::Printer::_LISP_STYLE) << std::endl;
		std::cout << "Abstract Syntax Tree" << std::endl << printer.print(lsbasi::Printer::_TREE) << std::endl;

		/** Intepret AST **/
		
		lsbasi::Interpreter interpreter(ast);
		std::cout << "Result -> " << interpreter.interpret() << std::endl;

	}

	return 0;
}
