#include <string>
#include <cctype>
#include <stdexcept>
#include <iostream>
#include <sstream> 

namespace lsbasi {

	struct Token {

		enum TokenType {
  			_INTEGER,
			_PLUS,
			_MINUS,
  			_MUL,
			_DIV,
    		_RPAREN,
    		_LPAREN,
			_ID,
			_EOF,
			_ASSIGN,
			_SEMI,
			_BEGIN,
			_END,
			_DOT
		};

		TokenType type;
		std::string value;
		Token(TokenType type, std::string value): type(type), value(value){}
	};

	class Lexer {

		std::string text;
		int pos;
		char current_char;

		int error () {
			throw std::runtime_error("Invalid character");
		}

		void advance () {
			pos++;
			if (pos > text.size() - 1)
				current_char = 0;
			else
				current_char = text[pos];
		}

		char peek () {
			int peek_pos = pos+1;
			if (pos + 1 > text.size() - 1)
				return 0;
			return text[pos + 1];
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

		std::string id () {
			std::string result;
			while((current_char != 0) && (std::isalpha(current_char) || std::isdigit(current_char))){
				result += current_char;
				advance();
			}
			return result;
		}

        public:

		Token * get_next_token () {
			while(current_char != 0){
				skip_whitespace();
				if (std::isalpha(current_char)){
					std::string word = id();
					if (word == "BEGIN")
						return new Token (Token::_BEGIN, word);
					else if (word == "END")
						return new Token (Token::_END, word);
					else
						return new Token (Token::_ID, word);
				}
				if (std::isdigit(current_char))
					return new Token (Token::_INTEGER, integer());
				if ((current_char == ':') && peek() == '='){
					advance();
					advance();
					return new Token (Token::_ASSIGN, ":=");
				}
				if (current_char == '+'){
					advance();
					return new Token (Token::_PLUS, "+");
				}
				if (current_char == '-'){
					advance();
					return new Token (Token::_MINUS, "-");
				}
				if (current_char == '*'){
					advance();
					return new Token (Token::_MUL, "*");
				}
				if (current_char == '/'){
					advance();
					return new Token (Token::_DIV, "/");
				}
				if (current_char == '('){
					advance();
					return new Token (Token::_LPAREN, "(");
				}
				if (current_char == ')'){
					advance();
					return new Token (Token::_RPAREN, ")");
				}
				if (current_char == ';'){
					advance();
					return new Token (Token::_SEMI, ";");
				}
				if (current_char == '.'){
					advance();
					return new Token (Token::_DOT, ".");
				}
				error();
			}
			return new Token(Token::_EOF, "");
		}

		Lexer(std::string text): text(text), pos(0), current_char(text[pos]) {}

	};

};