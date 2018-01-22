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
						return new Token(_INTEGER, integer());

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

			}

			void eat (TokenType type){
				if (current_token->type == type)
					current_token = get_next_token();
				else
					error();
			}


		public:

			Interpreter(std::string text): text(text), pos(0), current_token(NULL), current_char(text[pos]) {}

			int expr(){

				int result;

				current_token = get_next_token();
				Token * left = current_token;
				eat(_INTEGER);

				Token * op = current_token;
				if (op->type == _PLUS)
					eat(_PLUS);
				else
					eat(_MINUS);

				Token * right = current_token;
				eat(_INTEGER);

				if(op->type == _PLUS) 
					result = std::stoi(left->value) + std::stoi(right->value);
				else
					result = std::stoi(left->value) - std::stoi(right->value);
			
				delete right;
				delete op;
				delete left;

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