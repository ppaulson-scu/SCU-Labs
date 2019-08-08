# include "checker.h"
# include "lexer.h"
# include "tokens.h"
using namespace std;
static std::vector<class Scope> _table;
 
void insertSymbol(Symbol s){
	//cout << "inserting " << s.identifier() << " with type " << s.type() << endl;
	_table[_table.size()-1].insert(s);
}

void insertSymbol(Symbol s, int scope){
	//cout << "inserting " << s.identifier() << " with type " << s.type() << endl;
	_table[scope].insert(s);
}

void printError(int errornum, string name){
	switch(errornum){
		case 1:	report("redefinition of '%s'", name);
				break;
		case 2:	report("redeclaration of '%s'", name);
				break;
		case 3:	report("conflicting types for '%s'", name);
				break;
		case 4:	report("'%s' undeclared", name);
				break;
	};
}

void openScope(){
	cout << "Opening scope." << endl;
	_table.push_back(Scope());
}

void closeScope(){
	cout << "Closed scope." << endl;
	//_table.back().print();
	_table.pop_back();
}

void declareFunction(string ident, Type t){
	cout << "Func dec: " << ident << ":" << t << endl;
	for(int i = _table.size()-1; i >= 0; i--){
		if(_table[0].hasSymbol(ident)){
			Symbol* symbol = _table[0].find(ident);
			if(symbol->type() != t){
				printError(3, ident);
				cout << symbol->type() << " : " << t << endl;
			} else {
				return;
			}
		}
		break;
	}
	insertSymbol(Symbol(ident, t), 0);
}

void defineFunction(string ident, Type t){
	cout << "Func def: " << ident << ":" << t << endl;
	//for(int i = _table.size()-1; i >= 0; i--){
		if(_table[0].hasSymbol(ident)){					//DECLARED
			Symbol* symbol = _table[0].find(ident);
			cout << symbol->defined() << endl;
			_table[0].print();

			if(symbol->defined()){						//ALREADY DEFINED				
				printError(1, ident);
			} else if (symbol->type() != t){			//NONIDENTICAL DECLARATIONS
				cout << symbol->type() << " : " << t << endl;
				printError(3, ident);
			} else {									//INSERT DEFINITION
				symbol->define();
				insertSymbol(Symbol(ident, t, true), 0);
				//_table[0].print();				
			}
			//break;
		}
	//}
}

void declareVariable(string ident, Type t){
	cout << "Var dec: " << ident << ":" << t << endl;
	if(_table.size() == 1){						//GLOBAL
		cout << ident << endl;
		if(_table[0].hasSymbol(ident) && _table[0].find(ident)->type() != t){	//CONFLICTING DECLARATION
			printError(3, ident);
		}
	} else {									//LOCAL
		if(_table[_table.size()-1].hasSymbol(ident)){
			printError(2, ident);
		}
	}
	insertSymbol(Symbol(ident, t));
	//_table[0].print();
}

void checkIdentifier(string ident){
	for(int i = _table.size()-1; i >= 0; i--){
		if(_table[i].hasSymbol(ident)){
			return;
		}
	}
	printError(4, ident);
}

void checkFunction(string ident){
	if(_table[0].hasSymbol(ident)){
		//Symbol symbol = _table[0].find(ident);
		
	} else {
		cout << ident <<  " is implicit decl" << endl;
		_table[0].insert(Symbol(ident, Type(INT, 0, nullptr)));			//IMPLICIT
	}
}

