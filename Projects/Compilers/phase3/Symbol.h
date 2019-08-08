# ifndef SYMBOL_H
# define SYMBOL_H

# include "Type.h"
# include <string>


class Symbol {

		std::string _identifier;
		Type _type;
		bool _defined;

public:
		Symbol(std::string ident, Type t);
		Symbol(std::string ident, Type t, bool def);

		std::string identifier(){ return _identifier; }
		Type type(){ return _type; }
		bool defined(){ return _defined; }
		void define(){ _defined = true; }

};

# endif /* SYMBOL_H */
