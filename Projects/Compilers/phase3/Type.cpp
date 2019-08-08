# include <cassert>
# include "Type.h"


using namespace std;

//Constructors
Type::Type()
	:_kind(ERROR){}

Type::Type(int specifier, unsigned indirection)
	:_specifier(specifier), _indirection(indirection), _kind(SCALAR){}

Type::Type(int specifier, unsigned indirection, unsigned length)
	:_specifier(specifier), _indirection(indirection), _length(length), _kind(ARRAY){}

Type::Type(int specifier, unsigned indirection, Parameters *parameters)
	:_specifier(specifier), _indirection(indirection), _parameters(parameters), _kind(FUNCTION){}

//Operator overloads
bool Type::operator==(const Type &rhs) const {
	if(_kind != rhs._kind)
		return false;
	if(_kind == ERROR)
		return true;
	if(_specifier != rhs._specifier)
		return false;
	if(_indirection != rhs._indirection)
		return false;
	if(_kind == SCALAR)
		return true;
	if(_kind == ARRAY)
		return _length == rhs._length;
	if(!_parameters || !rhs._parameters)  //implicit function declarations
		return true;
	return *_parameters == *rhs._parameters;
}

bool Type::operator!=(const Type &rhs) const {
	return !operator==(rhs);
}

bool Type::isArray() const {
	return _kind == ARRAY;
}

bool Type::isFunction() const {
	return _kind == FUNCTION;
}

bool Type::isError() const {
	return _kind == ERROR;
}

bool Type::isScalar() const {
	return _kind == SCALAR;
}

unsigned Type::length() const {
	assert(_kind == ARRAY);
	return _length;
}

Parameters* Type::parameters() const {
	assert(_kind == FUNCTION);
	return _parameters;

}

std::ostream &operator<<(std::ostream &ostr, const Type& type){
	ostr << "Type(";
	if(type.isError()){
		ostr << "ERROR)";
	} else if(type.isScalar()){
		ostr << type.specifier() << ", " << type.indirection() << ", SCALAR)";
	} else if(type.isArray()){
		ostr << type.specifier() << ", " << type.indirection() << ", ARRAY, " << type.length() << ")";
	} else if(type.isFunction()) {
		ostr << type.specifier() << ", " << type.indirection() << ", FUNCTION, " << type.parameters() << ")";
	} else {
		ostr << ") print call failed miserably.";
	}
	return ostr;
}

std::ostream &operator<<(std::ostream &ostr, const Parameters& parameters){
	ostr << "(";
	for(Type t : parameters){
		ostr << t << " ";
	}
	ostr << ")" << endl;
	return ostr;
}
