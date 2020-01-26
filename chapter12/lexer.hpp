#include <string>
#include <cctype>
#include <stdexcept>
#include <iostream>
#include <sstream> 

namespace lsbasi {

	struct Token {

		enum TokenType {
  			_INTEGER,
			_REAL,
			_PLUS,
			_MINUS,
  			_MUL,
			_INT_DIV,
			_REAL_DIV,
    		_RPAREN,
    		_LPAREN,
			_ID,
			_EOF,
			_ASSIGN,
			_SEMI,
			_BEGIN,
			_END,
			_DOT,
			_TYPE_INTEGER,
			_TYPE_REAL,
			_VAR,
			_PROCEDURE,
			_PROGRAM,
			_COMMA,
			_COLON,

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

		void skip_comment () {
			while((current_char != 0) && (current_char != '}')) 
				advance();
			advance();
		}

		Token * number () {
			Token * token; 
			std::string result;
			while((current_char != 0) && std::isdigit(current_char)){
				result += current_char;
				advance();
			}

			if (current_char == '.'){
				result += current_char;
				advance();
				while((current_char != 0) && std::isdigit(current_char)){
					result += current_char;
					advance();
				}
				token = new Token(Token::_REAL, result);
			}else{
				token = new Token(Token::_INTEGER, result);
			}

			return token;
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
					else if (word == "INTEGER")
						return new Token (Token::_TYPE_INTEGER, word);  
					else if (word == "REAL")
						return new Token (Token::_TYPE_REAL, word);
					else if (word == "VAR")
						return new Token (Token::_VAR, word);
					else if (word == "PROCEDURE")
						return new Token (Token::_PROCEDURE, word);
					else if (word == "DIV")
						return new Token (Token::_INT_DIV, word);
					else if (word == "PROGRAM")
						return new Token (Token::_PROGRAM, word);
					else
						return new Token (Token::_ID, word);
				}
				if (current_char == '{'){
					advance();
					skip_comment();
					continue;
				}
				if (std::isdigit(current_char))
					return number();
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
					return new Token (Token::_REAL_DIV, "/");
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
				if (current_char == ':'){
					advance();
					return new Token (Token::_COLON, ":");
				}
				if (current_char == ','){
					advance();
					return new Token (Token::_COMMA, ",");
				}
				error();
			}
			return new Token(Token::_EOF, "");
		}

		Lexer(std::string text): text(text), pos(0), current_char(text[pos]) {}

	};

};