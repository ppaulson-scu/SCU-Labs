# include <string>
# include <iostream>
# include <cctype>

using namespace std;

string keywords[40] = {"auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum", "extern", "float", "for", "goto", "if", "int", "long", "register", "return", "short", "signed", "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"};

enum {
    KEYWORD, ID, INTEGER, REAL, STRING, OPERATOR, DONE,
};

int lexan(string &lexbuf)
{
	lexbuf.clear();

	//Comments and leading whitespace
	bool isComment = false;
	char input;
	do{
		input = cin.get();
		if(input == '/' && cin.peek() == '*'){
			isComment = true;
		}
		if(input == '*' && cin.peek() == '/'){
			cin.get();
			input = cin.get();
			isComment = false;
		}
	}while(isspace(input) || isComment);

	//Reals, integers
	if(isdigit(input)){
		lexbuf += input;
		while(isdigit(cin.peek())){
			lexbuf += cin.get();
		}
		if(cin.peek() == '.'){
			lexbuf += cin.get();
			while(isdigit(cin.peek())){
				lexbuf += cin.get();
			}
			if(cin.peek() == 'E' || cin.peek() == 'e'){
				lexbuf += cin.get();
				if(cin.peek() == '+' || cin.peek() == '-'){
					lexbuf += cin.get();
				}
				while(isdigit(cin.peek())){
					lexbuf += cin.get();
				}
			}
			return REAL;
		} else return INTEGER;
	}

	//Identifiers, keywords
	if(isalpha(input) || input == '_'){
		lexbuf += input;
		while(isalnum(cin.peek()) || cin.peek() == '_'){
			lexbuf += cin.get();
		}
		for(string s : keywords){
			if(s == lexbuf)
				return KEYWORD;
		}
		return ID;		
	}

	//Strings
	if(input == '"'){
		lexbuf += input;
		while (cin.peek() != '"'){
			lexbuf += cin.get();
		}
		lexbuf += cin.get();
		return STRING;
	}

	//Operators
	if(input == '='){
		lexbuf += input;
		if(cin.peek() == '='){
			lexbuf += cin.get();
		}
		return OPERATOR;
	}

	if(input == '|' && cin.peek() =='|'){
		lexbuf += input;
		lexbuf += cin.get();
		return OPERATOR;
	}

	if(input == '&'){
		lexbuf += input;
		if(cin.peek() == '&'){
			lexbuf += cin.get();
		}
		return OPERATOR;
	}

	if(input == '!'){
		lexbuf += input;
		if(cin.peek() == '='){
			lexbuf += cin.get();
		}
		return OPERATOR;
	}

	if(input == '<'){
		lexbuf += input;
		if(cin.peek() == '='){
			lexbuf += cin.get();
		}
		return OPERATOR;
	}

	if(input == '>'){
		lexbuf += input;
		if(cin.peek() == '='){
			lexbuf += cin.get();
		}
		return OPERATOR;
	}

	if(input == '+'){
		lexbuf += input;
		if(cin.peek() == '+'){
			lexbuf += cin.get();
		}
		return OPERATOR;
	}

	if(input == '-'){
		lexbuf += input;
		if(cin.peek() == '-' || cin.peek() == '>'){
			lexbuf += cin.get();
		}
		return OPERATOR;
	}

	if(input == '*'){
		lexbuf += input;
		return OPERATOR;
	}
	if(input == '/'){
		lexbuf += input;
		return OPERATOR;
	}
	if(input == '%'){
		lexbuf += input;
		return OPERATOR;
	}
	if(input == '.'){
		lexbuf += input;
		return OPERATOR;
	}
	if(input == ','){
		lexbuf += input;
		return OPERATOR;
	}
	if(input == '('){
		lexbuf += input;
		return OPERATOR;
	}
	if(input == ')'){
		lexbuf += input;
		return OPERATOR;
	}
	if(input == '{'){
		lexbuf += input;
		return OPERATOR;
	}
	if(input == '}'){
		lexbuf += input;
		return OPERATOR;
	}
	if(input == '['){
		lexbuf += input;
		return OPERATOR;
	}
	if(input == ']'){
		lexbuf += input;
		return OPERATOR;
	}
	if(input == ';'){
		lexbuf += input;
		return OPERATOR;
	}
	if(input == ':'){
		lexbuf += input;
		return OPERATOR;
	}

    return DONE;
}

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
	}

	cout << type << ":" << lexbuf << endl;
    }

    return 0;
}
