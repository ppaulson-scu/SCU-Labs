/*
 * File: parser.cpp
 *
 * Description: This file contains the function definitions
 * 		for a grammatical parser for Simple C.
 *
 */

# include <set>
# include <cstdio>
# include <cctype>
# include <cerrno>
# include <string>
# include <cstdlib>
# include <iostream>
# include <map>
# include "parser.h"
# include "token.h"
# include "lexer.h"

using namespace std;

static int lookahead;
static string lexbuf;

void error(){
	report("syntax error");
	exit(EXIT_FAILURE);
}

void match(int c){
	if (lookahead != c) {
		error();
	}
	lookahead = lexan(lexbuf);
} 

int main() {
	lookahead = lexan(lexbuf);
	while(lookahead != DONE){
		parse();
	}
	
	exit(1);
}

void parse(){
	func_or_global();
}

void expression(){
	and_expression();
	while(lookahead == OR) {
		match(OR);
		and_expression();
		cout << "or" << endl;
	}
}

void and_expression(){
	equality_expression();
	while(lookahead == AND) {
		match(AND);
		equality_expression();
		cout << "and" << endl;
	}	
}

void equality_expression(){
	comparison_expression();
	while(lookahead == EQUALS || lookahead == NOTEQUALS){
		int token = lookahead;
		match(lookahead);
		comparison_expression();
		if(token == EQUALS) {
			cout << "eql" << endl;
		} else if (token == NOTEQUALS) {
			cout << "neq" << endl;
		}
	}
}

void comparison_expression(){
	add_expression();
	while(lookahead == LESSEQUALS || lookahead == GREATEQUALS || lookahead == GT || lookahead == LT){
		int token = lookahead;	
		match(lookahead);
		add_expression();
		if(token == LESSEQUALS){
			cout << "leq" << endl;	
		} else if (token == GREATEQUALS){
			cout << "geq" << endl;
		} else if (token == GT) {
			cout << "gtn" << endl;
		} else if (token == LT) {
			cout << "ltn" << endl;
		}
	}
}

void add_expression(){
	mult_expression();
	while(lookahead == ADD || lookahead == NEGATE){
		int token = lookahead;
		match(lookahead);
		mult_expression();
		if(token == ADD){
			cout << "add" << endl;
		} else if (token == NEGATE) {
			cout << "sub" << endl;
		}
	}
}

void mult_expression(){
	unary_expression();
	while(lookahead == STAR || lookahead == DIV || lookahead == MOD){
		int token = lookahead;
		match(lookahead);
		unary_expression();
		if(token == STAR){
			cout << "mul" << endl;	
		} else if (token == DIV){
			cout << "div" << endl;
		} else if (token == MOD) {
			cout << "rem" << endl;
		}
	}
}

void unary_expression(){
	if(lookahead == STAR || lookahead == NEGATE || lookahead == NOT || lookahead == ADDR){
		int token = lookahead;
		match(lookahead);
		unary_expression();
		if(token == STAR){
			cout << "deref" << endl;	
		} else if (token == NEGATE){
			cout << "neg" << endl;
		} else if (token == NOT) {
			cout << "not" << endl;
		} else if (token == ADDR) {
			cout << "addr" << endl;
		}
	} else if (lookahead == SIZEOF){
		cout << "sizeof" << endl;
		match(lookahead);
		match(OPENPAREN);
		specifier();
		pointers();
		match(CLOSEPAREN);
	} else if (lookahead == OPENPAREN){
		match(lookahead);
		if(lookahead == CHAR || lookahead == DOUBLE || lookahead == INT){
			specifier();
			pointers();
			match(CLOSEPAREN);
			unary_expression();
			cout << "cast" << endl;
		} else { 
			expression();
			match(CLOSEPAREN);
			array_expression();
		}	
	} else {
		primary_expression();
		array_expression();
	} 
}

void array_expression(){
	while(lookahead == OPENBRACKET){
		match(lookahead);
		expression();
		match(CLOSEBRACKET);
		cout << "index" << endl;
	}
}

void primary_expression(){
	if(lookahead == ID){
		match(ID);
		if(lookahead == OPENPAREN){
			match(lookahead);
			expression_list();
			match(CLOSEPAREN);
		}
	} else if (lookahead == REAL || lookahead == INTEGER || lookahead == STRING){
		match(lookahead);
	} else if (lookahead == OPENPAREN){
		match(lookahead);
		expression();
		match(CLOSEPAREN);
	}
}

void expression_list(){
	expression();
	while(lookahead == COMMA){
		match(lookahead);
		expression();
	}
}

void specifier(){
	if(lookahead == CHAR || lookahead == INT || lookahead == DOUBLE){
		match(lookahead);
	} else {
		error();
	}
}

void pointers(){
	if(lookahead == STAR){
		match(lookahead);
		pointers();
	}
}

void statements(){
	while(lookahead != CLOSEBRACE){
		statement();
	}
}

void statement(){
	if(lookahead == OPENBRACE){
		match(lookahead);
		declarations();
		statements();
		match(CLOSEBRACE);
	} else if (lookahead == RETURN){;
		match(lookahead);
		expression();
		match(SEMI);
	} else if (lookahead == WHILE){
		match(lookahead);
		match(OPENPAREN);
		expression();
		match(CLOSEPAREN);
		statement();
	} else if (lookahead == IF){;
		match(lookahead);
		match(OPENPAREN);
		expression();
		match(CLOSEPAREN);
		statement();
		if(lookahead == ELSE) {
			match(lookahead);
			statement();
		}
	} else {
		expression();
		if (lookahead == ASSIGN){
			match(lookahead);
			expression();
		}
		match(SEMI);
	}
}

void declarations(){
	while(lookahead == CHAR || lookahead == INT || lookahead == DOUBLE){
		declaration();
	}
}

void declaration(){
	specifier();
	declarator();
	while(lookahead == COMMA){
		match(lookahead);
		declarator();
	}
	match(SEMI);
}

void declarator(){
	pointers();
	match(ID);
	if(lookahead == OPENBRACKET){
		match(lookahead);
		match(INTEGER);
		match(CLOSEBRACKET);
	}
	
}

void func_or_global(){
	specifier();
	pointers();
	match(ID);
	if(lookahead == OPENBRACKET){
		match(lookahead);
		match(INTEGER);
		match(CLOSEBRACKET);
		global_declarators();
	} else if (lookahead == OPENPAREN){
		match(lookahead);
		parameters();
		match(CLOSEPAREN);
		
		if(lookahead == OPENBRACE){
			match(OPENBRACE);
			declarations();
			statements();
			match(CLOSEBRACE);
		} else { 
			global_declarators();
		}
	} else { 
		global_declarators();
	}
}

void global_declarators(){
	if(lookahead == SEMI){
		match(lookahead);
	} else if (lookahead == COMMA){
		match(lookahead);
		global_declarator();
		global_declarators();
	}
}

void global_declarator(){
	pointers();
	match(ID);
	if(lookahead == OPENPAREN){
		match(lookahead);
		parameters();
		match(CLOSEPAREN);
	} else if (lookahead == OPENBRACKET){
		match(lookahead);
		match(INTEGER);
		match(CLOSEBRACKET);
	}
}

void parameters(){
	if(lookahead == VOID){
		match(lookahead);
	} else {
		parameter_list();
	}
}

void parameter_list(){
	specifier();
	pointers();
	match(ID);
	while(lookahead == COMMA){
		match(lookahead);
		specifier();
		pointers();
		match(ID);
	}
}
