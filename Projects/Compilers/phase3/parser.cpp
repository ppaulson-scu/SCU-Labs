/*
 * File:	parser.cpp
 *
 * Description:	This file contains the public and private function and
 *		variable definitions for the recursive-descent parser for
 *		Simple C.
 */

# include <cstdlib>
# include <iostream>
# include <vector>
# include "tokens.h"
# include "lexer.h"
# include "checker.h"

using namespace std;

static int lookahead;
static string lexbuf;

static void expression();
static void statement();


/*
 * Function:	error
 *
 * Description:	Report a syntax error to standard error.
 */

static void error()
{
    if (lookahead == DONE)
	report("syntax error at end of file");
    else
	report("syntax error at '%s'", lexbuf);

    exit(EXIT_FAILURE);
}


/*
 * Function:	match
 *
 * Description:	Match the next token against the specified token.  A
 *		failure indicates a syntax error and will terminate the
 *		program since our parser does not do error recovery.
 */

static void match(int t)
{
    if (lookahead != t)
	error();

    lookahead = lexan(lexbuf);
}


/*
 * Function:	isSpecifier
 *
 * Description:	Return whether the given token is a type specifier.
 */

static bool isSpecifier(int token)
{
    return token == CHAR || token == INT || token == DOUBLE;
}

static string name() {
	string buf = lexbuf;
	match(ID);
	return buf;
}

static unsigned integer() {
	string buf = lexbuf;
	match(INTEGER);
	return stoul(buf, nullptr);
}

/*
 * Function:	specifier
 *
 * Description:	Parse a type specifier.  Simple C has char, int, and long.
 *
 *		specifier:
 *		  char
 *		  int
 *		  double
 */

static int specifier()
{
	int typespec = lookahead;
	if(lookahead == INT){
		match(INT);
	} else if(lookahead == DOUBLE){
		match(DOUBLE);
	} else {
		match(CHAR); 
	}
	return typespec;
}


/*
 * Function:	pointers
 *
 * Description:	Parse pointer declarators (i.e., zero or more asterisks).
 *
 *		pointers:
 *		  empty
 *		  * pointers
 */

static unsigned pointers()
{
	unsigned count = 0;
    while (lookahead == '*'){
		count++;
		match('*');
	}
	return count;
}


/*
 * Function:	declarator
 *
 * Description:	Parse a declarator, which in Simple C is either a scalar
 *		variable or an array, with optional pointer declarators.
 *
 *		declarator:
 *		  pointers identifier
 *		  pointers identifier [ integer ]
 */

static void declarator(int typespec)
{
	unsigned indirection = pointers();
    string ident = name();

    if (lookahead == '[') {
		match('[');
		//match(INTEGER);
		unsigned length = integer();
		match(']');
		declareVariable(ident, Type(typespec, indirection, length));
		//cout << ident << ":" << Type(typespec, indirection, length) << endl;
		//cout << ident << ":(" << typespec << ", " << indirection << ", " << length << ", ARRAY)" << endl;
    } else {
		declareVariable(ident, Type(typespec, indirection)); 
		//cout << ident << ":" << Type(typespec, indirection) << endl;
		//cout << ident << ":(" << typespec << ", " << indirection << ", SCALAR)" << endl;
	}
}


/*
 * Function:	declaration
 *
 * Description:	Parse a local variable declaration.  Global declarations
 *		are handled separately since we need to detect a function
 *		as a special case.
 *
 *		declaration:
 *		  specifier declarator-list ;
 *
 *		declarator-list:
 *		  declarator
 *		  declarator , declarator-list
 */

static void declaration()
{
    int typespec = specifier();
    declarator(typespec);

    while (lookahead == ',') {
		match(',');
		declarator(typespec);
    }

    match(';');
}


/*
 * Function:	declarations
 *
 * Description:	Parse a possibly empty sequence of declarations.
 *
 *		declarations:
 *		  empty
 *		  declaration declarations
 */

static void declarations()
{
    while (isSpecifier(lookahead))
	declaration();
}


/*
 * Function:	primaryExpression
 *
 * Description:	Parse a primary expression.
 *
 *		primary-expression:
 *		  ( expression )
 *		  identifier ( expression-list )
 *		  identifier ( )
 *		  identifier
 *		  string
 *		  integer
 *		  real
 *
 *		expression-list:
 *		  expression
 *		  expression , expression-list
 */

static void primaryExpression(bool lparenMatched)
{
    if (lparenMatched) {
	expression();
	match(')');

    } else if (lookahead == STRING) {
		match(STRING);

    } else if (lookahead == INTEGER) {
		match(INTEGER);

    } else if (lookahead == REAL) {
		match(REAL);

    } else if (lookahead == ID) {
		string str = lexbuf;
		match(ID);

		if (lookahead == '(') {
			checkFunction(str);
	   		match('(');

	    	if (lookahead != ')') {
				expression();

				while (lookahead == ',') {
		    		match(',');
		    		expression();
				}
	    	}

	    	match(')');
		} else {
			checkIdentifier(str);
		}
    } else 
	error();
}


/*
 * Function:	postfixExpression
 *
 * Description:	Parse a postfix expression.
 *
 *		postfix-expression:
 *		  primary-expression
 *		  postfix-expression [ expression ]
 */

static void postfixExpression(bool lparenMatched)
{
    primaryExpression(lparenMatched);

    while (lookahead == '[') {
	match('[');
	expression();
	match(']');
	cout << "index" << endl;
    }
}


/*
 * Function:	prefixExpression
 *
 * Description:	Parse a prefix expression.
 *
 *		prefix-expression:
 *		  postfix-expression
 *		  ! prefix-expression
 *		  - prefix-expression
 *		  * prefix-expression
 *		  & prefix-expression
 *		  sizeof ( specifier pointers )
 *		  ( specifier pointers ) prefix-expression
 */

static void prefixExpression()
{
    if (lookahead == '!') {
	match('!');
	prefixExpression();
	cout << "not" << endl;

    } else if (lookahead == '-') {
	match('-');
	prefixExpression();
	cout << "neg" << endl;

    } else if (lookahead == '*') {
	match('*');
	prefixExpression();
	cout << "deref" << endl;

    } else if (lookahead == '&') {
	match('&');
	prefixExpression();
	cout << "addr" << endl;

    } else if (lookahead == SIZEOF) {
	match(SIZEOF);
	match('(');
	specifier();
	pointers();
	match(')');
	cout << "sizeof" << endl;

    } else if (lookahead == '(') {
	match('(');

	if (isSpecifier(lookahead)) {
	    specifier();
	    pointers();
	    match(')');
	    prefixExpression();
	    cout << "cast" << endl;

	} else
	    postfixExpression(true);

    } else
	postfixExpression(false);
}


/*
 * Function:	multiplicativeExpression
 *
 * Description:	Parse a multiplicative expression.
 *
 *		multiplicative-expression:
 *		  prefix-expression
 *		  multiplicative-expression * prefix-expression
 *		  multiplicative-expression / prefix-expression
 *		  multiplicative-expression % prefix-expression
 */

static void multiplicativeExpression()
{
    prefixExpression();

    while (1) {
	if (lookahead == '*') {
	    match('*');
	    prefixExpression();
	    cout << "mul" << endl;

	} else if (lookahead == '/') {
	    match('/');
	    prefixExpression();
	    cout << "div" << endl;

	} else if (lookahead == '%') {
	    match('%');
	    prefixExpression();
	    cout << "rem" << endl;

	} else
	    break;
    }
}


/*
 * Function:	additiveExpression
 *
 * Description:	Parse an additive expression.
 *
 *		additive-expression:
 *		  multiplicative-expression
 *		  additive-expression + multiplicative-expression
 *		  additive-expression - multiplicative-expression
 */

static void additiveExpression()
{
    multiplicativeExpression();

    while (1) {
	if (lookahead == '+') {
	    match('+');
	    multiplicativeExpression();
	    cout << "add" << endl;

	} else if (lookahead == '-') {
	    match('-');
	    multiplicativeExpression();
	    cout << "sub" << endl;

	} else
	    break;
    }
}


/*
 * Function:	relationalExpression
 *
 * Description:	Parse a relational expression.  Note that Simple C does not
 *		have shift operators, so we go immediately to additive
 *		expressions.
 *
 *		relational-expression:
 *		  additive-expression
 *		  relational-expression < additive-expression
 *		  relational-expression > additive-expression
 *		  relational-expression <= additive-expression
 *		  relational-expression >= additive-expression
 */

static void relationalExpression()
{
    additiveExpression();

    while (1) {
	if (lookahead == '<') {
	    match('<');
	    additiveExpression();
	    cout << "ltn" << endl;

	} else if (lookahead == '>') {
	    match('>');
	    additiveExpression();
	    cout << "gtn" << endl;

	} else if (lookahead == LEQ) {
	    match(LEQ);
	    additiveExpression();
	    cout << "leq" << endl;

	} else if (lookahead == GEQ) {
	    match(GEQ);
	    additiveExpression();
	    cout << "geq" << endl;

	} else
	    break;
    }
}


/*
 * Function:	equalityExpression
 *
 * Description:	Parse an equality expression.
 *
 *		equality-expression:
 *		  relational-expression
 *		  equality-expression == relational-expression
 *		  equality-expression != relational-expression
 */

static void equalityExpression()
{
    relationalExpression();

    while (1) {
		if (lookahead == EQL) {
	    	match(EQL);
	    	relationalExpression();
		    	cout << "eql" << endl;

		} else if (lookahead == NEQ) {
		    match(NEQ);
		    relationalExpression();
		    cout << "neq" << endl;

		} else {
	    	break;
		}
    }
}


/*
 * Function:	logicalAndExpression
 *
 * Description:	Parse a logical-and expression.  Note that Simple C does
 *		not have bitwise-and expressions.
 *
 *		logical-and-expression:
 *		  equality-expression
 *		  logical-and-expression && equality-expression
 */

static void logicalAndExpression()
{
    equalityExpression();

    while (lookahead == AND) {
		match(AND);
		equalityExpression();
		cout << "and" << endl;
    }
}


/*
 * Function:	expression
 *
 * Description:	Parse an expression, or more specifically, a logical-or
 *		expression, since Simple C does not allow comma or
 *		assignment as an expression operator.
 *
 *		expression:
 *		  logical-and-expression
 *		  expression || logical-and-expression
 */

static void expression()
{
    logicalAndExpression();

    while (lookahead == OR) {
		match(OR);
		logicalAndExpression();
		cout << "or" << endl;
    }
}


/*
 * Function:	statements
 *
 * Description:	Parse a possibly empty sequence of statements.  Rather than
 *		checking if the next token starts a statement, we check if
 *		the next token ends the sequence, since a sequence of
 *		statements is always terminated by a closing brace.
 *
 *		statements:
 *		  empty
 *		  statement statements
 */

static void statements()
{
    while (lookahead != '}')
	statement();
}


/*
 * Function:	statement
 *
 * Description:	Parse a statement.  Note that Simple C has so few
 *		statements that we handle them all in this one function.
 *
 *		statement:
 *		  { declarations statements }
 *		  return expression ;
 *		  while ( expression ) statement
 *		  if ( expression ) statement
 *		  if ( expression ) statement else statement
 *		  expression = expression ;
 *		  expression ;
 */

static void statement()
{
    if (lookahead == '{') {
		match('{');
		openScope();
		//cout << "Opening statement scope. {" << endl;
		declarations();
		statements();
		closeScope();
		//cout << "} Closed statement scope." << endl;
		match('}');

    } else if (lookahead == RETURN) {
		match(RETURN);
		expression();
		match(';');

    } else if (lookahead == WHILE) {
		match(WHILE);
		match('(');
		expression();
		match(')');
		statement();

    } else if (lookahead == IF) {
		match(IF);
		match('(');
		expression();
		match(')');
		statement();

	if (lookahead == ELSE) {
	    match(ELSE);
	    statement();
	}

    } else {
		expression();

		if (lookahead == '=') {
		    match('=');
		    expression();
	}
		match(';');
    }
}


/*
 * Function:	parameter
 *
 * Description:	Parse a parameter, which in Simple C is always a scalar
 *		variable with optional pointer declarators.
 *
 *		parameter:
 *		  specifier pointers ID
 */

static Type parameter()
{
    int typespec = specifier();
    unsigned indirection = pointers();
    string ident = name();
	Type parameter_type = Type(typespec, indirection);
	declareVariable(ident, parameter_type);
	//cout << ident << ":" << parameter_type << endl;
	//cout << ident << ": (" << typespec << ", " << indirection << ", SCALAR)" << endl;
	return parameter_type;
}


/*
 * Function:	parameters
 *
 * Description:	Parse the parameters of a function, but not the opening or
 *		closing parentheses.
 *
 *		parameters:
 *		  void
 *		  parameter-list
 *
 *		parameter-list:
 *		  parameter
 *		  parameter , parameter-list
 */

static void parameters(Parameters *function_parameters)
{
    if (lookahead == VOID){
		match(VOID);
    } else {
		function_parameters->insert(function_parameters->begin(), parameter());

		while (lookahead == ',') {
	    	match(',');
	    	function_parameters->insert(function_parameters->begin(), parameter());
		}
    }
}


/*
 * Function:	globalDeclarator
 *
 * Description:	Parse a declarator, which in Simple C is either a scalar
 *		variable, an array, or a function, with optional pointer
 *		declarators.
 *
 *		global-declarator:
 *		  pointers identifier
 *		  pointers identifier ( parameters )
 *		  pointers identifier [ integer ]
 */

static void globalDeclarator(int typespec)
{
    unsigned indirection = pointers();
    string ident = name();

    if (lookahead == '(') {
		match('(');
		openScope();
		Parameters function_parameters;
		parameters(&function_parameters);
		match(')');
		closeScope();
		declareFunction(ident, Type(typespec, indirection, &function_parameters));
		//cout << ident << ":" << Type(typespec, indirection, &function_parameters) << endl;
		//cout << ident << ":(INT, " << indirection << ", SCALAR)" << endl;
    } else if (lookahead == '[') {
		match('[');
		//checkIdentifier(ident);
		unsigned length = integer();
		match(']');
		declareVariable(ident, Type(typespec, indirection, length));
		//cout << ident << ":(INT, " << indirection << ", " << length << ", ARRAY)" << endl;
    } else {
		declareVariable(ident, Type(typespec, indirection));
		cout << ident << ":(" << typespec << ", " << indirection << ", SCALAR)" << endl;
	}
}


/*
 * Function:	remainingDeclarators
 *
 * Description:	Parse any remaining global declarators after the first.
 *
 * 		remaining-declarators
 * 		  ;
 * 		  , global-declarator remaining-declarators
 */

static void remainingDeclarators(int typespec)
{
    while (lookahead == ',') {
		match(',');
		globalDeclarator(typespec);
    }

    match(';');
}


/*
 * Function:	globalOrFunction
 *
 * Description:	Parse a global declaration or function definition.
 *
 * 		global-or-function:
 * 		  specifier pointers identifier remaining-decls
 * 		  specifier pointers identifier [ integer ] remaining-decls
 * 		  specifier pointers identifier ( parameters ) remaining-decls 
 * 		  specifier pointers identifier ( parameters ) { ... }
 */

static void globalOrFunction()
{
    int typespec = specifier();
    unsigned indirection = pointers();
   	string ident = name();

    if (lookahead == '[') {
		match('[');
		//match(INTEGER);
		unsigned length = integer();
		match(']');
		declareVariable(ident, Type(typespec, indirection, length));
		//cout << ident << ":" << Type(typespec, indirection, length) << endl;
		//cout << ident << ":(INT, " << indirection << ", " << length << ", ARRAY)" << endl;
		remainingDeclarators(typespec);
    } else if (lookahead == '(') {
		//cout << "Opened scope of function " << ident << ". {" << endl;
		openScope();
		Parameters function_parameters;
		match('(');
		parameters(&function_parameters);
		match(')');
		declareFunction(ident, Type(typespec, indirection, &function_parameters));

		if (lookahead == '{') {
			defineFunction(ident, Type(typespec, indirection, &function_parameters));
	   		match('{');
	    	declarations();
	    	statements();
			closeScope();
			//cout << "} Closed scope of function definition " << ident << "." << endl;
	    	match('}');

		} else {
			closeScope();
			//cout << "} Closed scope of function declaration " << ident << "." << endl;
	    	remainingDeclarators(typespec);
		}

    } else {
		declareVariable(ident, Type(typespec, indirection));
		remainingDeclarators(typespec);
	}

}


/*
 * Function:	main
 *
 * Description:	Analyze the standard input stream.
 */

int main()
{
    lookahead = lexan(lexbuf);
	openScope();
    while (lookahead != DONE){
		globalOrFunction();
	}

	closeScope();
    exit(EXIT_SUCCESS);
}
