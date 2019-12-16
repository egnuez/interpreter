// GRAMMAR:
// expr: term ((PLUS|MINUS) term)*
// term: fact ((MUL|DIV) factor)*
// fact: (PLUS|MINUS) fact | NUM | LPAREN expr RPAREN | ID
// statement_compound: BEGIN statement_list END
// statement_list: (statement SEMI)*
// statement: ID ASSIGN expr 

// GENERATE:
// expr: term {% var node = $1;} ((PLUS|MINUS) term {% node = new BinOp(left=node, op=$token, right=$2); })* {% return node; }
// term: fact {% var node = $1;} ((MUL|DIV) factor {% node = new BinOp(left=node, op=$token, right=$2); })* {% return node; }
// fact: (PLUS|MINUS) fact {% return new UnaryOp($token, $1);} | NUM {% return new Num($token);} | LPAREN expr RPAREN {% return $1;}
//

// TERMINALS:
// BEGIN:   BEGIN
// END:     END
// NUM:     [0-9]+
// ID:      [a-zA-Z]+[0-9]*
// ASSIGN:  :=
// DIV:     /
// MUL:     *
// PLUS:    +
// MINUS:   -
// LPAREN:  (
// RPAREN:  )
// DOT:     .
// SEMI:    ; 

function inArray(needle, haystack) {
    var length = haystack.length;
    for(var i = 0; i < length; i++) {
        if(haystack[i] == needle) return true;
    }
    return false;
}

/**
 * AST
 */

var Num = function(token){
    this._name_ = "Num";
    this.token = token;
}

var BinOp = function(expr_left, token, expr_right){
    this._name_ = "BinOp";
    this.token = token;
    this.expr_left = expr_left;
    this.expr_right = expr_right;
}

var UnaryOp = function(token, expr){
    this._name_ = "UnaryOp";
    this.token = token;
    this.expr = expr;
}

var Compound = function(children){
    this._name_ = "Compound";
    this.children = children;
}

var Assign = function(id, expr){
    this._name_ = "Assign";
    this.id = id;
    this.expr = expr;
}

var Id = function(token){
    this._name_ = "Id";
    this.token = token;
}

/**
 * Tokens
 */

Tokens = {
    T_EOF:      0,
    T_BEGIN:    1,
    T_END:      2,
    T_NUM:      3,
    T_ID:       4,
    T_ASSIGN:   5,
    T_DIV:      6,
    T_MUL:      7,
    T_PLUS:     8,
    T_MINUS:    9,
    T_LPAREN:   10,
    T_RPAREN:   11,
    T_DOT:      12,
    T_SEMI:     13
}

TokensRex = [
    /^$/,
    /^BEGIN/,
    /^END/,
    /^[0-9]+/,
    /^[a-zA-Z]+[0-9]*/,
    /^:=/,
    /^\//,
    /^\*/,
    /^\+/,
    /^\-/,
    /^\(/,
    /^\)/,
    /^\./,
    /^;/
]

var Token = function(type, valor){
    this.type = type;
    this.valor = valor;
}

/**
 * Lexer
 */

var Lexer = function(text){

    this.text = text;

    this.getNextToken = function() {

        this.text = this.text.replace(/^\s*/, "");

        for(var token_type = 0; token_type < TokensRex.length; token_type++) {
            let chunk = this.text.match(TokensRex[token_type]);
            if(chunk){
                this.text = this.text.substring(chunk[0].length);
                return new Token(token_type, chunk[0]);
            }
        }

        console.log("Lexer Error");
        process.exit(1);

    };
}

/**
 * Parser
 */

var Parser = function (lexer){

    this.lexer = lexer;
    this.currentToken = lexer.getNextToken();
    
    this.eat = function(type) {
        const ret = this.currentToken;
        if (ret.type == type){
            this.currentToken = lexer.getNextToken();
        }else{
            console.log("Parser Error I");
            process.exit(1);
        }
        return ret;
    }

    /**
     * Automatic methods
     */

    this.expr = function() {
        
        let $1 = this.term();

        /** Add by {% ... } */
        var node = $1;
        /** End */

        while(inArray(this.currentToken.type, [Tokens.T_PLUS, Tokens.T_MINUS])){
            let $token = this.currentToken;
            this.eat($token.type);
            let $2 = this.term();
            
            /** Add by {% ... } */
            node = new BinOp(node, $token, $2);
            /** End */
        }

        /** Add by {% ... } */
        return node;
        /** End */

    }

    this.term = function() {

        let $1 = this.fact();

        /** Add by {% ... } */
        var node = $1;
        /** End */

        while(inArray(this.currentToken.type, [Tokens.T_MUL, Tokens.T_DIV])){
            let $token = this.currentToken;
            this.eat($token.type);
            let $2 = this.fact();

            /** Add by {% ... } */
            node = new BinOp(node, $token, $2);
            /** End */
        }

        /** Add by {% ... } */
        return node;
        /** End */

    }

    this.fact = function() {
        $token = this.currentToken;
        if(inArray(this.currentToken.type, [Tokens.T_PLUS, Tokens.T_MINUS])){
            this.eat(this.currentToken.type);
            let $1 = this.fact();

            /**
             * Add by {% ... }
             */

            return new UnaryOp($token, $1);

            /** */

        }else if (this.currentToken.type == Tokens.T_NUM){
            this.eat(Tokens.T_NUM);

            /** Add by {% ... } */
            return new Num($token);
            /** End */

        }else if(this.currentToken.type == Tokens.T_LPAREN){
            this.eat(Tokens.T_LPAREN);
            let $1 = this.expr();
            this.eat(Tokens.T_RPAREN);

            /** Add by {% ... } */
            return $1;
            /** End */

        }else{
            console.log("Parser Error III");
            process.exit(1);
        }
    }

    this.statement_compound = function() {
        this.eat(Tokens.T_BEGIN);
        let $1 = this.statement_list();
        this.eat(Tokens.T_END);
        return new Compound($1);
    }

    this.statement_list = function() {
        let stlist = [];
        while(this.currentToken.type == Tokens.T_ID){
            stlist.push(this.statement());
            this.eat(Tokens.T_SEMI);
        }
        return stlist;
    }

    this.statement = function(){
        let id = this.eat(Tokens.T_ID);
        this.eat(Tokens.T_ASSIGN);
        return new Assign(id, this.expr());
    }

    /** End */

    this.parse = function(){
        let res = this.statement_compound();
        if (this.currentToken.type != Tokens.T_EOF){
            console.log("Parser Error IV");
            process.exit(1);
        }
        return res;
    }

}

/**
 * Interpreter
 */

var Interpreter = function (parser){
    this.parser = parser;
    this.visit = function(ast){
        const method_name = 'visit_' + ast._name_;
        return this[method_name](ast);
    }
    this.visit_Num = function(ast){
        return parseInt(ast.token.valor);
    }
    this.visit_BinOp = function(ast){
        switch(ast.token.type){
            case Tokens.T_PLUS:
                return this.visit(ast.expr_left) + this.visit(ast.expr_right);
            case Tokens.T_MINUS:
                return this.visit(ast.expr_left) - this.visit(ast.expr_right);
            case Tokens.T_MUL:
                return this.visit(ast.expr_left) * this.visit(ast.expr_right);
            case Tokens.T_DIV:
                return this.visit(ast.expr_left) / this.visit(ast.expr_right);
        }
    }
    this.visit_UnaryOp = function(ast){
        if(ast.token.type = Tokens.T_MINUS){
            return -1 * this.visit(ast.expr);
        }else{
            return this.visit(ast.expr);
        }
    }
    this.interpret = function(){
        ast = this.parser.parse();
        return this.visit(ast);
    }
}

const lexer = new Lexer("BEGIN var1 := 1; var2:=2+2; END");
const parser = new Parser(lexer);
console.dir(parser.parse(), {depth:null});
//const interpreter = new Interpreter(parser);
//console.log(interpreter.interpret());