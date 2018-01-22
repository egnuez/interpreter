#include <string>
#include <cctype>
#include <stdexcept>
#include <iostream>

namespace lsbasi {

	enum TokenType {
  		_INTEGER,
  		_PLUS,
  		_EOF
	};

	class Token{
		public:
			TokenType type;
			char value;
			Token(TokenType type, char value): type(type), value(value){}
	};

	class Interpreter {

		private:

			std::string text;
			int pos;
			Token * current_token;

			int error(){
				throw std::runtime_error("Error parsing input");
			}

			Token * get_next_token(){

				if (pos > text.size() - 1)
					return new Token(_EOF, 0);

				char current_char = text[pos];

				if (std::isdigit(current_char)){
					pos++;
					return new Token (_INTEGER, current_char);
				}

				if (current_char == '+'){
					pos++;
					return new Token (_PLUS, current_char);
				}

				error();

			}

			void eat (TokenType type){
				if (current_token->type == type)
					current_token = get_next_token();
				else
					error();
			}


		public:

			Interpreter(std::string text): text(text), pos(0), current_token(NULL){}

			int expr(){

				current_token = get_next_token();
				Token * left = current_token;
				eat(_INTEGER);

				Token * op = current_token;
				eat(_PLUS);

				Token * right = current_token;
				eat(_INTEGER);

				int result = (left->value - '0') + (right->value - '0');
			
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