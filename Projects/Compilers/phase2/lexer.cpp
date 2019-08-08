/*
 * File:	lexer.cpp
 *
 * Description:	This file contains the public and private function and
 *		variable definitions for the lexical analyzer for Simple C.
 *
 *		Extra functionality:
 *		- checking for improper integer and floating-point literals
 */

# include <set>
# include <cstdio>
# include <cctype>
# include <cerrno>
# include <string>
# include <cstdlib>
# include <iostream>
# include <map>
# include "lexer.h"
# include "token.h"

using namespace std;
int numerrors, lineno = 1;

/* enum {
    KEYWORD, ID, INTEGER, REAL, STRING, OPERATOR, DONE,
}; */


/* Later, we will associate token values with each keyword */

static map<string, int> keywords = {
    {"auto", AUTO},
    {"break", BREAK},
    {"case", CASE},
    {"char", CHAR},
    {"const", CONST},
    {"continue", CONTINUE},
    {"default", DEFAULT},
    {"do", DO},
    {"double", DOUBLE},
    {"else", ELSE},
    {"enum", ENUM},
    {"extern", EXTERN},
    {"float", FLOAT},
    {"for", FOR},
    {"goto", GOTO},
    {"if", IF},
    {"int", INT},
    {"long", LONG},
    {"register", REGISTER},
    {"return", RETURN},
    {"short", SHORT},
    {"signed", SIGNED},
    {"sizeof", SIZEOF},
    {"static", STATIC},
    {"struct", STRUCT},
    {"switch", SWITCH},
    {"typedef", TYPEDEF},
    {"union", UNION},
    {"unsigned", UNSIGNED},
    {"void", VOID},
    {"volatile", VOLATILE},
    {"while", WHILE},
};


/*
 * Function:	report
 *
 * Description:	Report an error to the standard error prefixed with the
 *		line number.  We'll be using this a lot later with an
 *		optional string argument, but C++'s stupid streams don't do
 *		positional arguments, so we actually resort to snprintf.
 *		You just can't beat C for doing things down and dirty.
 */

void report(const string &str, const string &arg)
{
    char buf[1000];

    snprintf(buf, sizeof(buf), str.c_str(), arg.c_str());
    cerr << "line " << lineno << ": " << buf << endl;
    numerrors ++;
}


/*
 * Function:	lexan
 *
 * Description:	Read and tokenize the standard input stream.  The lexeme is
 *		stored in a buffer.
 */

int lexan(string &lexbuf)
{
    long val;
    static int c = cin.get();


    /* The invariant here is that the next character has already been read
       and is ready to be classified.  In this way, we eliminate having to
       push back characters onto the stream, merely to read them again. */

    while (!cin.eof()) {
	lexbuf.clear();


	/* Ignore white space */

	while (isspace(c)) {
	    if (c == '\n')
		lineno ++;

	    c = cin.get();
	}


	/* Check for an identifier or a keyword */

	if (isalpha(c) || c == '_') {
	    do {
		lexbuf += c;
		c = cin.get();
	    } while (isalnum(c) || c == '_');

	    if (keywords.count(lexbuf) > 0)
			return keywords[lexbuf];

	    return ID;


	/* Check for a number (integer or real). */

	} else if (isdigit(c)) {
	    do {
		lexbuf += c;
		c = cin.get();
	    } while (isdigit(c));

	    if (c != '.') {
		errno = 0;
		val = strtol(lexbuf.c_str(), NULL, 0);

		if (errno != 0 || val != (int) val)
		    report("integer constant too large");

		return INTEGER;
	    }

	    lexbuf += c;
	    c = cin.get();

	    if (isdigit(c)) {
		do {
		    lexbuf += c;
		    c = cin.get();
		} while (isdigit(c));

		if (c == 'e' || c == 'E') {
		    lexbuf += c;
		    c = cin.get();

		    if (c == '-' || c == '+') {
			lexbuf += c;
			c = cin.get();
		    }

		    if (isdigit(c)) {
			do {
			    lexbuf += c;
			    c = cin.get();
			} while (isdigit(c));
		    } else
			report("missing exponent of floating-point constant");
		}
	    } else
		report("missing fractional part of floating-point constant");

	    errno = 0;
	    strtod(lexbuf.c_str(), NULL);

	    if (errno != 0)
		report("floating-point constant out of range");

	    return REAL;


	/* There must be an easier way to do this.  It might seem stupid at
	   this point to recognize each token separately, but eventually
	   we'll be returning separate token values to the parser, so we
	   might as well do it now. */

	} else {
	    lexbuf += c;

	    switch(c) {


	    /* Check for '||' */

	    case '|':
		c = cin.get();

		if (c == '|') {
		    lexbuf += c;
		    c = cin.get();
		    return OR;
		}

		break;


	    /* Check for '=' and '==' */

	    case '=':
		c = cin.get();

		if (c == '=') {
		    lexbuf += c;
		    c = cin.get();
			return EQUALS;
		}

		return '=';


	    /* Check for '&' and '&&' */

	    case '&':
		c = cin.get();

		if (c == '&') {
		    lexbuf += c;
		    c = cin.get();
			return AND;
		}

		return '&';


	    /* Check for '!' and '!=' */

	    case '!':
		c = cin.get();

		if (c == '=') {
		    lexbuf += c;
		    c = cin.get();
			return NOTEQUALS;
		}

		return '!';


	    /* Check for '<' and '<=' */

	    case '<':
		c = cin.get();

		if (c == '=') {
		    lexbuf += c;
		    c = cin.get();
			return LESSEQUALS;
		}

		return '<';


	    /* Check for '>' and '>=' */

	    case '>':
		c = cin.get();

		if (c == '=') {
		    lexbuf += c;
		    c = cin.get();
			return GREATEQUALS;
		}

		return '>';


	    /* Check for '-', '--', and '->' */

	    case '-':
		c = cin.get();

		if (c == '-') {
		    lexbuf += c;
		    c = cin.get();
			return DECREMENT;
		} else if (c == '>') {
		    lexbuf += c;
		    c = cin.get();
			return ARROW;
		}

		return '-';


	    /* Check for '+' and '++' */

	    case '+':
		c = cin.get();

		if (c == '+') {
		    lexbuf += c;
		    c = cin.get();
			return INCREMENT;
		}

		return '+';


	    /* Check for simple, single character tokens */

	    case '*': case '%': case ':': case ';':
	    case '(': case ')': case '[': case ']':
	    case '{': case '}': case '.': case ',':
		{
		int r = c;
		c = cin.get();
		return r;
		}


	    /* Check for '/' or a comment */

	    case '/':
		c = cin.get();

		if (c == '*') {
		    c = cin.get();

		    do {
			while (c != '*' && !cin.eof()) {
			    if (c == '\n')
				lineno ++;

			    c = cin.get();
			}

			c = cin.get();
		    } while (c != '/' && !cin.eof());

		    c = cin.get();
		    break;

		} else
		    return '/';


	    /* Check for a string literal */

	    case '"':
		c = cin.get();

		while (c != '"' && c != '\n' && !cin.eof()) {
		    lexbuf += c;
		    c = cin.get();
		}

		if (c == '\n' || cin.eof())
		    report("premature end of string literal");

		lexbuf += c;
		c = cin.get();
		return STRING;


	    /* Handle EOF here as well */

	    case EOF:
		return DONE;


	    /* Ignore everything else */

	    default:
		c = cin.get();
		break;
	    }
	}
    }

    return DONE;
}


/*
 * Function:	main
 *
 * Description:	Read and tokenize and standard input stream.
 */

/*
int main()
{
    int token;
    string lexbuf, type;

    while ((token = lexan(lexbuf)) != DONE) {
	switch(token) {
	case ID:
	    type = "identifier";
	    break;

	case KEYWORD:
	    type = "keyword";
	    break;

	case INTEGER:
	    type = "integer";
	    break;

	case REAL:
	    type = "real";
	    break;

	case STRING:
	    type = "string";
	    break;

	case OPERATOR:
	    type = "operator";
	    break;

	case AUTO:
	    type = "keyword";
	    break;

	case BREAK:
	    type = "keyword";
	    break;

	case CASE:
	    type = "keyword";
	    break;

	case CHAR:
	    type = "keyword";
	    break;

	case CONST:
	    type = "keyword";
	    break;

	case CONTINUE:
	    type = "keyword";
	    break;

	case DEFAULT:
	    type = "keyword";
	    break;

	case DO:
	    type = "keyword";
	    break;

	case DOUBLE:
	    type = "keyword";
	    break;

	case ELSE:
	    type = "keyword";
	    break;

	case ENUM:
	    type = "keyword";
	    break;

	case EXTERN:
	    type = "keyword";
	    break;

	case FLOAT:
	    type = "keyword";
	    break;

	case FOR:
	    type = "keyword";
	    break;

	case GOTO:
	    type = "keyword";
	    break;

	case IF:
	    type = "keyword";
	    break;

	case INT:
	    type = "keyword";
	    break;
	
	case LONG:
	    type = "keyword";
	    break;

	case REGISTER:
	    type = "keyword";
	    break;

	case RETURN:
	    type = "keyword";
	    break;

	case SHORT:
	    type = "keyword";
	    break;

	case SIGNED:
	    type = "keyword";
	    break;

	case SIZEOF:
	    type = "keyword";
	    break;

	case STATIC:
	    type = "keyword";
	    break;

	case STRUCT:
	    type = "keyword";
	    break;

	case SWITCH:
	    type = "keyword";
	    break;

	case TYPEDEF:
	    type = "keyword";
	    break;

	case UNION:
	    type = "keyword";
	    break;

	case UNSIGNED:
	    type = "keyword";
	    break;

	case VOID:
	    type = "keyword";
	    break;

	case VOLATILE:
	    type = "keyword";
	    break;

	case WHILE:
	    type = "keyword";
	    break;

	case AND:
	    type = "operator";
	    break;

	case EQUALS:
	    type = "operator";
	    break;

	case NOTEQUALS:
	    type = "operator";
	    break;

	case LESSEQUALS:
	    type = "operator";
	    break;

	case GREATEQUALS:
	    type = "operator";
	    break;

	case DECREMENT:
	    type = "operator";
	    break;

	case ARROW:
	    type = "operator";
	    break;

	case INCREMENT:
	    type = "operator";
	    break;

	case ASSIGN:
	    type = "operator";
	    break;

	case BINAND:
	    type = "operator";
	    break;

	case NOT:
	    type = "operator";
	    break;

	case LT:
	    type = "operator";
	    break;

	case GT:
	    type = "operator";
	    break;

	case NEGATE:
	    type = "operator";
	    break;

	case ADD:
	    type = "operator";
	    break;

	case STAR:
	    type = "operator";
	    break;

	case MOD:
	    type = "operator";
	    break;

	case COLON:
	    type = "operator";
	    break;

	case SEMI:
	    type = "operator";
	    break;

	case OPENPAREN:
	    type = "operator";
	    break;

	case CLOSEPAREN:
	    type = "operator";
	    break;

	case OPENBRACKET:
	    type = "operator";
	    break;

	case CLOSEBRACKET:
	    type = "operator";
	    break;

	case OPENBRACE:
	    type = "operator";
	    break;

	case CLOSEBRACE:
	    type = "operator";
	    break;

	case DOT:
	    type = "operator";
	    break;

	case COMMA:
	    type = "operator";
	    break;
	}

	cout << type << ":" << lexbuf << endl;
    }

    return 0;
} */
