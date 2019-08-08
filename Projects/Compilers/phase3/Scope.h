# ifndef SCOPE_H
# define SCOPE_H

# include <vector>
# include <string>
# include "Symbol.h"
# include <iostream>
typedef std::vector<class Symbol> Symbols;

class Scope {

	Symbols _symbols;

public:

	Scope();
	
	void print();
	void insert(Symbol s);
	void remove(std::string name);
	Symbol *find(std::string name);
	bool hasSymbol(std::string name);
};

# endif /* SCOPE_H */
