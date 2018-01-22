#include <string>
#include <cctype>
#include <stdexcept>
#include <iostream>

namespace lsbasi {

	enum TokenType {
  		_INTEGER,
  		_PLUS,
		_MINUS,
  		_EOF
	};

	class Token{
		public:
			TokenType type;
			std::string value;
			Token(TokenType type, std::string value): type(type), value(value){}
	};

	class Interpreter {

		private:

			std::string text;
			int pos;
			Token * current_token;
			char current_char;

			int error () {
				throw std::runtime_error("Error parsing input");
			}

			void advance () {
				pos++;
				if (pos > text.size() - 1)
					current_char = 0;
				else
					current_char = text[pos];
			}

			void skip_whitespace () {
				while((current_char != 0) && (current_char == ' ')) 
					advance();
			}

			std::string integer () {
				std::string result;
				while((current_char != 0) && std::isdigit(current_char)){
					result += current_char; 
					advance();
				}
				return result;
			}

			Token * get_next_token () {

				while(current_char != 0){
					skip_whitespace();
					if (std::isdigit(current_char))
						return new Token (_INTEGER, integer());

					if (current_char == '+'){
						advance();
						return new Token (_PLUS, "+");
					}

					if (current_char == '-'){
						advance();
						return new Token (_MINUS, "-");
					}

					error();

				}

				return new Token(_EOF, "");

			}

			void eat (TokenType type){
				if (current_token->type == type){
					delete current_token;
					current_token = get_next_token();
				}else
					error();
			}

			int term(){
				std::string res = current_token->value;
				eat(_INTEGER);
				return std::stoi(res);
			}


		public:

			Interpreter(std::string text): text(text), pos(0), current_token(NULL), current_char(text[pos]) {}

			int expr(){

				current_token = get_next_token();

				int result = term();

				while((current_token->type == _PLUS) || (current_token->type == _MINUS)){
				
					Token * tk = current_token;
					if (tk->type == _PLUS){
						eat(_PLUS);
						result += term(); 
					}
					if (tk->type == _MINUS){
						eat(_MINUS);
						result -= term(); 
					}
				}

				return result;
			}

	};

};

int main (){

	while(true){

		std::string text;
		std::cout << "calc> "; 
		std::cin >> text;
		lsbasi::Interpreter interpreter(text);
		std::cout << interpreter.expr() << std::endl;

		//ctrl+c to exit

	}

	return 0;
}