/*
 * File:	checker.cpp
 *
 * Description:	This file contains the public and private function and
 *		variable definitions for the semantic checker for Simple C.
 *
 *		Extra functionality:
 *		- inserting an undeclared symbol with the error type
 */

# include <set>
# include <iostream>
# include "lexer.h"
# include "checker.h"
# include "tokens.h"
# include "Symbol.h"
# include "Scope.h"
# include "Type.h"


using namespace std;

static set<string> funcdefns;
static Scope *outermost, *toplevel;
static const Type error;

static string redefined = "redefinition of '%s'";
static string redeclared = "redeclaration of '%s'";
static string conflicting = "conflicting types for '%s'";
static string undeclared = "'%s' undeclared";

void printError(int errornum, string input = ""){
	switch(errornum){
		case 1: report("invalid return type", "");
				break;
		case 2:	report("invalid type for test expression", "");
				break;
		case 3: report("lvalue required in expression", "");
				break;
		case 4: report("invalid operands to binary %s", input);
				break;
		case 5: report("invalid operand to unary %s", input);
				break;
		case 6: report("invalid operand in cast expression", "");
				break;
		case 7: report("called object is not a function", "");
				break;
		case 8: report("invalid arguments to called function", "");
				break;
		default:
				break;
	};
}

/*
 * Function:	openScope
 *
 * Description:	Create a scope and make it the new top-level scope.
 */

Scope *openScope()
{
    toplevel = new Scope(toplevel);

    if (outermost == nullptr)
	outermost = toplevel;

    return toplevel;
}


/*
 * Function:	closeScope
 *
 * Description:	Remove the top-level scope, and make its enclosing scope
 *		the new top-level scope.
 */

Scope *closeScope()
{
    Scope *old = toplevel;

    toplevel = toplevel->enclosing();
    return old;
}


/*
 * Function:	defineFunction
 *
 * Description:	Define a function with the specified NAME and TYPE.  A
 *		function is always defined in the outermost scope.  Any
 *		previous declaration is discarded.
 */

Symbol *defineFunction(const string &name, const Type &type)
{
    Symbol *symbol = outermost->find(name);

    if (funcdefns.count(name) > 0) {
	report(redefined, name);
	outermost->remove(name);
	delete symbol->type().parameters();
	delete symbol;
    }

    symbol = declareFunction(name, type);
    funcdefns.insert(name);
    return symbol;
}


/*
 * Function:	declareFunction
 *
 * Description:	Declare a function with the specified NAME and TYPE.  A
 *		function is always declared in the outermost scope.  Any
 *		previous declaration is discarded.
 */

Symbol *declareFunction(const string &name, const Type &type)
{
    Symbol *symbol = outermost->find(name);

    if (symbol != nullptr) {
	if (type != symbol->type())
	    report(conflicting, name);

	if (symbol->type().isFunction())
	    delete symbol->type().parameters();

	outermost->remove(name);
	delete symbol;
    }

    symbol = new Symbol(name, type);
    outermost->insert(symbol);
    return symbol;
}


/*
 * Function:	declareVariable
 *
 * Description:	Declare a variable with the specified NAME and TYPE.  Any
 *		previous declaration is discarded.
 */

Symbol *declareVariable(const string &name, const Type &type)
{
    Symbol *symbol = toplevel->find(name);

    if (symbol != nullptr) {
	if (outermost != toplevel)
	    report(redeclared, name);
	else if (type != symbol->type())
	    report(conflicting, name);

	if (symbol->type().isFunction())
	    delete symbol->type().parameters();

	toplevel->remove(name);
	delete symbol;
    }

    symbol = new Symbol(name, type);
    toplevel->insert(symbol);
    return symbol;
}


/*
 * Function:	checkIdentifier
 *
 * Description:	Check if NAME is declared.  If it is undeclared, then
 *		declare it as having the error type in order to eliminate
 *		future error messages.
 */

Symbol *checkIdentifier(const string &name)
{
    Symbol *symbol = toplevel->lookup(name);

    if (symbol == nullptr) {
	report(undeclared, name);
	symbol = new Symbol(name, error);
	toplevel->insert(symbol);
    }

    return symbol;
}


/*
 * Function:	checkFunction
 *
 * Description:	Check if NAME is a previously declared function.  If it is
 *		undeclared, then implicitly declare it.
 */

Symbol *checkFunction(const string &name)
{
    Symbol *symbol = toplevel->lookup(name);

    if (symbol == nullptr)
		symbol = declareFunction(name, Type(INT, 0, nullptr));

    return symbol;
}

Type checkLogical(const Type &left, const Type &right, const int op)
{
	if(left == error || right == error)
		return error;
	if(left.promote().isPredicate() && right.promote().isPredicate()){
		return Type(INT);
	} else {
		if(op == AND) 
			printError(4, "&&");
		if(op == OR)
			printError(4, "||");
		if(!left.promote().isPredicate()){
			cout << "	" << left << " is not a predicate type (left)." << endl;
		}
		if(!right.promote().isPredicate()){
			cout << "	" << right << " is not a predicate type (right)." << endl;
		}
	}
	return error;
}

Type checkEquality(const Type &left, const Type &right, const int op)
{
	if(left == error || right == error)
		return error;
	if(left.promote().isCompatibleWith(right.promote())) {
		return Type(INT);
	} else {
		if(op == EQL)
			printError(4, "==");
		if(op == NEQ)
			printError(4, "!=");
		cout << "	" << left << " is not compatible with " << right << "." << endl;
	}
	return error;
}

Type checkRelational(const Type &left, const Type &right, const int op)
{
	if(left == error || right == error)
		return error;
	if(left.promote().isCompatibleWith(right.promote())) {
		return Type(INT);
	} else {
		if(op == LTN)
			printError(4, "<");
		if(op == GTN)
			printError(4, ">");
		if(op == LEQ)
			printError(4, "<=");
		if(op == GEQ)
			printError(4, ">=");
		cout << "	" << left << " is not compatible with " << right << "." << endl;
	}
	return error;
}

Type checkAdditive(const Type &left, const Type &right, const int op)
{
	if(left == error || right == error)
		return error;
	Type leftPromote = left.promote();
	Type rightPromote = right.promote();
	if(leftPromote.isNumeric() && rightPromote.isNumeric()){
		if(leftPromote.specifier() == DOUBLE || rightPromote.specifier() == DOUBLE){
			return Type(DOUBLE);
		} else {
			return Type(INT);
		}
	}
	cout << "	" << leftPromote << endl;
	cout << "	" << ((op == MINUS) ? "-" : "+") <<  endl;
	cout << "	" << rightPromote << endl;
	cout << endl;
	if(leftPromote.isPointer() && rightPromote == Type(INT)){
		return left;
	}
	if(op == PLUS){
		if(leftPromote.specifier() == INT && rightPromote.isPointer() && !leftPromote.isPointer()){
			return right;
		}
		printError(4, "+");
		cout << "	" << left << " plus " << right << " is not a valid addition expression." << endl;
	}
	if(op == MINUS){
		if(rightPromote.isPointer() && leftPromote.isPointer() && leftPromote.specifier() == rightPromote.specifier()){
			return Type(INT);
		}
		printError(4, "-");
		cout << "	" << left << " minus " << right << " is not a valid subtraction expression." << endl;
	}
	return error;
}

Type checkMultiplicative(const Type &left, const Type &right, const int op)
{
	if(left == error || right == error)
		return error;
	if(left.promote().isNumeric() && right.promote().isNumeric()){
		if(left.specifier() == DOUBLE || right.specifier() == DOUBLE){
			return Type(DOUBLE);
		} else {
			return Type(INT);
		}
	} else {
		if(op == STAR)
			printError(4, "*");
		if(op == DIV)
			printError(4, "/");
		if(!left.promote().isNumeric()){
			cout << "	" << left << " is not a numeric type (left)." << endl;
		}
		if(!right.promote().isNumeric()){
			cout << "	" << right << " is not a numeric type (right)." << endl;
		}
	}
	return error;
}

Type checkRemainder(const Type &left, const Type &right)
{
	if(left == error || right == error)
		return error;
	if(left.promote().specifier() == INT && right.promote().specifier() == INT)
		return Type(INT);
	printError(4, "%");
	if(!(left.promote().specifier() == INT)){
		cout << "	" << left << " is not a integer type (left)." << endl;
	}
	if(!(right.promote().specifier() == INT)){
		cout << "	" << left << " is not a integer type (right)." << endl;
	}
	return error;		
}

Type checkPrefix(const Type &right, const int op, bool isLvalue)
{
	if(right == error) return error;
	if(op == '*'){
		if(!(right.promote().isPointer())){
			printError(5, "*");
			cout << "	" << right << " is not a pointer for * operation." << endl;
		} else {
			return Type(right.promote().specifier(), right.promote().indirection() - 1);
		}
	} else if(op == '&') {
		if(!isLvalue){
			printError(3, "&");
			cout << "	" << right << " is not an lvalue for & operation." << endl;
		} else {
			return Type(right.specifier(), right.indirection() + 1);
		}
	} else if(op == '!') {
		if(!(right.isPredicate())){
			cout << "	" << right << " is not a predicate type for ! operation." << endl;
			printError(5, "!");
		} else {
			return Type(INT);
		}
	} else if(op == '-') {
		if(!(right.isNumeric())){
			cout << "	" << right << " is not a numeric type for - operation." << endl;
			printError(5, "-");
		} else {
			return right.promote();
		}
	} else if(op == SIZEOF) {
		return Type(INT);
	}
	return error;
}

Type checkCast(const Type &result, const Type &operand)
{
	if(result == error || operand == error)
		return error;
	Type resultPromote = result.promote();
	Type operandPromote = operand.promote();
	cout << "	casting " << operand << " to " << result << "." <<endl;
	if(resultPromote.isNumeric() && operandPromote.isNumeric())
		return result;
	if(resultPromote.isPointer() && operandPromote.isPointer())
		return result;
	if(resultPromote.specifier() == INT && operandPromote.isPointer())
		return result;
	if(resultPromote.isPointer() && operandPromote.specifier() == INT)
		return result;
	printError(6, "cast");
	cout << "	" << operand << " cannot be cast to " << result << "." << endl;
	return error;
}

Type checkPostfix(const Type &left, const Type &right)
{
	if(left == error || right == error)
		return error;
	if(left.promote().isPointer() && right.promote() == Type(INT))
		return Type(left.promote().specifier(), left.promote().indirection() - 1);
	printError(4, "[]");
	if(!left.promote().isPointer()){
		cout << "	" << left << " is not a pointer when promoted." << endl;
	}
	if(right.promote() == Type(INT)){
		cout << "	" << right << " is not an INT type when promoted." << endl;
	}
	return error;
}

Type checkFunctionTypes(const string name, Parameters &inputParams)
{
	Symbol *s = checkFunction(name);
	Type declared = s->type();
	if(!declared.isFunction()){
		printError(7);
		cout << "	" << declared << " is not a function." << endl;
		return error;
	};
	Parameters promotedInputParams;
	for(Type t : inputParams){
		promotedInputParams.push_back(t.promote());
	}
	Type called = Type(declared.specifier(), declared.indirection(), &promotedInputParams);
	/*
	if(declared.parameters() && *declared.parameters() != *called.parameters()){
		printError(8);
		cout << "	" << called << " has parameters " << promotedInputParams << ". It should have parameters " << *declared.parameters() << endl;
		return error;
	}
	*/
	if(declared.parameters() && declared.parameters()->size() != called.parameters()->size()){
		for(int i = 0; i < declared.parameters()->size(); i++){
			if(!declared.parameters()->at(i).promote().isCompatibleWith(called.parameters()->at(i).promote()));
				printError(8);
				return error;
		}
	}
	/*
	if(called != declared){
		printError(8);
		cout << "	" << called << " has parameters " << inputParams << ". It should have parameters " << *declared.parameters()<< endl;
		return error;
	}
	*/
	return Type(declared.specifier(), declared.indirection());
}

Type checkTest(const Type &testExpr)
{
	if(testExpr == error)
		return error;
	cout << "	" << testExpr << endl;
	if(!testExpr.isPredicate()){
		printError(2);
		cout << "	" << testExpr << " is not a predicate type." << endl;
		return error;
	}
	return testExpr;
}

Type checkAssignment(const Type &left, const Type &right, const bool isLValue)
{
	if(left == error || right == error)
		return error;
	if(!isLValue){
		printError(3);
		cout << "	" << left << " is not an lvalue." << endl;
		return error;
	} else if (!left.promote().isCompatibleWith(right.promote())){
		printError(4, "=");
		cout << "	" << left.promote() << " is not compatible with " << right.promote() << "." << endl;
		return error;
	} else {
		cout << "	" << left.promote() << " is compatible with " << right.promote() << "." << endl;
		return left;
	}
}

Type checkReturn(const Type &right, const Type &funcType)
{
	if(right == error || funcType == error)
		return error;
	if(!right.promote().isCompatibleWith(funcType)){
		printError(1);
		cout << "	" << right << " is not compatible with " << funcType << "." << endl;
		return error;
	}
	return funcType;
}
