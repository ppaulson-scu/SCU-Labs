#ifndef TYPE_H
#define TYPE_H
 
#include <vector>
#include <ostream>
typedef std::vector<class Type> Parameters;
 
class Type {
                int _specifier;
                unsigned _indirection;
                unsigned _length;
                Parameters *_parameters;
 
enum { ARRAY = 0, ERROR, FUNCTION, SCALAR } _kind;
 
public:
				//Constructors
                Type();                                                                 //ERROR
                Type(int specifier, unsigned indirection = 0);                          //SCALAR
                Type(int specifier, unsigned indirection, unsigned length);             //ARRAY
                Type(int specifier, unsigned indirection, Parameters *parameters);      //FUNCTION
 
                bool operator==(const Type&rhs) const;                                 	//unmodifiable by reference
                bool operator!=(const Type&rhs) const;
                bool isArray() const;
                bool isFunction() const;
                bool isError() const;
                bool isScalar() const;
 
                int specifier() const { return _specifier; }
                unsigned indirection() const { return _indirection; }
                unsigned length() const;
                Parameters *parameters() const;
};
 
std::ostream &operator<<(std::ostream &ostr, const Type& type);
std::ostream &operator<<(std::ostream &ostr, const Parameters& parameters);
#endif   /*TYPE_H*/

