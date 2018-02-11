#include <iostream>
#include <sstream>
#include "lexer.hpp"
#include "parser.hpp"
#include "printer.hpp"
#include "interpreter.hpp"

int main (){

	//while(true){
		
		try{

			/** Read Input File **/

			std::stringstream text;
			for (std::string line; std::getline(std::cin, line);) {
        		text << " " << line;
    		}
			
			/** Apply Lexer and Parser, Return AST **/
		
			lsbasi::Lexer lexer(text.str());
			lsbasi::Parser parser(lexer);
			lsbasi::AST *ast = parser.parse();

			/** Print AST in three different formats **/
		
			lsbasi::Printer printer(ast);
			std::cout << printer.print() << std::endl;

			/** Intepret AST **/
		
			lsbasi::Interpreter interpreter(ast);
			interpreter.interpret();
			std::cout << interpreter.environment() << std::endl;

		} catch(std::exception &e){
			std::cout << e.what() << std::endl;
		}

	//}

	return 0;
}