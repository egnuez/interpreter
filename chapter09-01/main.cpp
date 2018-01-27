#include <iostream>
#include "lexer.hpp"
#include "parser.hpp"
#include "printer.hpp"
#include "interpreter.hpp"

int main (){

	while(true){
		
		try{

			/** Read Input Expression **/

			std::string text;
			std::cout << "calc> ";
			std::cin >> text;

			/** Apply Lexer and Parser, Return AST **/
		
			lsbasi::Lexer lexer(text);
			lsbasi::Parser parser(lexer);
			lsbasi::AST *ast = parser.parse();

			/** Print AST in three different formats **/
		
			lsbasi::Printer printer(ast);
			std::cout << printer.print(lsbasi::Printer::_TREE) << std::endl;

			/** Intepret AST **/
		
			lsbasi::Interpreter interpreter(ast);
			std::cout << interpreter.interpret() << std::endl;

		} catch(std::exception &e){
			std::cout << e.what() << std::endl;
		}
	}

	return 0;
}