#include "Symbol.h"

using namespace std;

Symbol::Symbol(string ident, Type t)
	:_identifier(ident), _type(t), _defined(false){}

Symbol::Symbol(string ident, Type t, bool def)
	:_identifier(ident), _type(t), _defined(def){}
