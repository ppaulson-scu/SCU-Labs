# include "Scope.h"

using namespace std;

Scope::Scope()
	:_symbols(){}


void Scope::insert(Symbol s){
	_symbols.push_back(s);
	//cerr << s.defined() << endl;
}

void Scope::remove(string name){
	for(int i = _symbols.size() - 1; i >=0; i--){
		if(!(_symbols[i].identifier().compare(name))){
			_symbols.erase(_symbols.begin() + i);
		}
	}
}

Symbol *Scope::find(string name){
	for(int i = _symbols.size() - 1; i >=0; i--){
		if(!(_symbols[i].identifier().compare(name))){
			return &_symbols[i];
		}
	}
	return nullptr;
}

bool Scope::hasSymbol(string name){
	for(int i = _symbols.size() - 1; i >=0; i--){
		if(!(_symbols[i].identifier().compare(name))){
			return true;
		}
	}
	return false;
}

void Scope::print(){
	for(Symbol s : _symbols){
		cout << s.identifier() << ":" << s.type() << "  " << s.defined() << endl;
	}
	cout << endl;
}
