
namespace lsbasi {

    class ParserOnly {

        Token * current_token;
        Lexer lexer;

        int error (std::string description) {
            throw std::runtime_error(description);
        }

        void eat (Token::TokenType type){
            if (current_token->type == type)
                current_token = lexer.get_next_token();
            else
                error("Error 0 => " + current_token->value);
        }

        void factor(){
            
            /*
            factor
                : INTEGER 
                | REAL
                | RPARENT expr LPARNET
                | PLUS factor 
                | MINUS factor
                | ID
                ;
            */

            Token * tk;
            tk = current_token;

            switch(current_token->type){
                case Token::_INTEGER:
                    eat(Token::_INTEGER);
                    break;
                case Token::_REAL:
                    eat(Token::_REAL);
                    break;
                case Token::_LPAREN:
                    eat(Token::_LPAREN);
                    expr();
                    eat(Token::_RPAREN);
                    break;
                case Token::_PLUS:
                    eat(Token::_PLUS);
                    factor();
                    break;
                case Token::_MINUS:
                    eat(Token::_MINUS);
                    factor();
                    break;
                case Token::_ID:
                    eat(Token::_ID);
                    break;
                default:
                    error("Incorrect Factor");
            }
            
        }

        void term(){
            
            /*
            term
                : factor ((MUL | REAL_DIV | INT_DIV ) factor ) *
                ;
            */
            
            factor();

            while((current_token->type == Token::_MUL) 
            || (current_token->type == Token::_INT_DIV)
            || (current_token->type == Token::_REAL_DIV)){
                    
                Token * tk;
                tk = current_token;

                if (current_token->type == Token::_MUL)
                    eat(Token::_MUL);
                else if (current_token->type == Token::_INT_DIV)
                    eat(Token::_INT_DIV);
                else if (current_token->type == Token::_REAL_DIV)
                    eat(Token::_REAL_DIV);

                factor();
            }

        }

        void expr(){
            
            /*
            expr 
                : term ((MINUS | PLUS) term ) *
                ;
            
            */
            
            term();

            while((current_token->type == Token::_PLUS) || (current_token->type == Token::_MINUS)){

                Token * tk;
                tk = current_token;

                if (current_token->type == Token::_PLUS)
                    eat(Token::_PLUS);

                else if (current_token->type == Token::_MINUS)
                    eat(Token::_MINUS);

                term();

            }

        }

        void assignment_statement(){
            
            /*
            assignment_statement
                : ID ASSIGN expr
                ;
            */
            
            eat(Token::_ID);
            eat(Token::_ASSIGN);
            expr();
        }

        void empty(){
            
            /*
            empty
                : -
                ;
            */
            
        }

        void statement(){
            
            /*
            statement
                : assignment_statement 
                | compound_statement 
                | empty
                ;
            */

            if(current_token->type == Token::_ID){
                assignment_statement();
            }else if(current_token->type == Token::_BEGIN){
                compound_statement();
            }else{
                empty();
            }
            
        }

        void statement_list(){
            
            /*
            statement_list
                : statement (SEMI statement)*
                ;
            */
            
            statement();
            while(current_token->type == Token::_SEMI){
                eat(Token::_SEMI);
                statement();
            }
        }

        void compound_statement(){
            
            /*
            compound_statement
                : BEGIN statement_list END
                ;
            */
            
            eat(Token::_BEGIN);
            statement_list();
            eat(Token::_END);
        }

        void variable_declaration(){
            
            /*
            variable_declaration
                : ID (COMMA ID)* COLON (TYPE_INTEGER | TYPE_REAL)
                ;
            */

            eat(Token::_ID);
            while(current_token->type == Token::_COMMA){
                eat(Token::_COMMA);
                eat(Token::_ID);
            }
            eat(Token::_COLON);
            if ((current_token->type == Token::_TYPE_INTEGER) || 
                (current_token->type == Token::_TYPE_REAL))
                eat(current_token->type);
            else error("Incorrect Type");
        }

        void declarations(){
            
            /*
            declarations
                : VAR (variable_declaration SEMI)+
                | (PROCEDURE ID LPAREN formal_parameter_list RPAREN SEMI block SEMI)*
                | empty
                ;
            */
            
            if(current_token->type == Token::_VAR){
                eat(Token::_VAR);
                variable_declaration();
                eat(Token::_SEMI);
                while (current_token->type == Token::_ID){
                    variable_declaration();
                    eat(Token::_SEMI);
                }
            }

            while(current_token->type == Token::_PROCEDURE){
                eat(Token::_PROCEDURE);
                eat(Token::_ID);
                eat(Token::_LPAREN);
                formal_parameter_list();
                eat(Token::_RPAREN);
                eat(Token::_SEMI);
                block();
                eat(Token::_SEMI);
            }

        }

        void formal_parameter_list(){

            /*
            formal_parameter_list 
                : variable_declaration (SEMI variable_declaration)*
                | empty
            */

            if (current_token->type == Token::_ID) {
                variable_declaration();
                while (current_token->type == Token::_COMMA){
                    eat(Token::_COMMA);
                    variable_declaration();
                }
            }
        }

        void block(){
            /*
            block
                : declarations compound_statement
                ;
            */
            
            declarations();
            compound_statement();
        }

        void program(){
            
            /*
            program
                : PROGRAM ID SEMI block DOT
                ;
            */
            
            eat(Token::_PROGRAM);
            eat(Token::_ID);
            eat(Token::_SEMI);
            block();
            eat(Token::_DOT);
            
        }

    public:

        ParserOnly(lsbasi::Lexer& lexer): lexer(lexer) {
            current_token = this->lexer.get_next_token();
        }

        void parse(){
            program();
            if (current_token->type != Token::_EOF)
                error("Error 2");
        }

    };
}