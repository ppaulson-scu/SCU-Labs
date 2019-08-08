/*
 * File:	checker.h
 *
 * Description:	This file contains the public function declarations for the
 *		semantic checker for Simple C.
 */

# ifndef CHECKER_H
# define CHECKER_H
# include "Scope.h"

Scope *openScope();
Scope *closeScope();

Symbol *defineFunction(const std::string &name, const Type &type);
Symbol *declareFunction(const std::string &name, const Type &type);
Symbol *declareVariable(const std::string &name, const Type &type);
Symbol *checkIdentifier(const std::string &name);
Symbol *checkFunction(const std::string &name);

Type checkLogical(const Type &left, const Type &right, const int op);
Type checkEquality(const Type &left, const Type &right, const int op);
Type checkRelational(const Type &left, const Type &right, const int op);
Type checkAdditive(const Type &left, const Type &right, const int op);
Type checkMultiplicative(const Type &left, const Type &right, const int op);
Type checkRemainder(const Type &left, const Type &right);
Type checkPrefix(const Type &right, const int op, bool isLvalue = true);
Type checkCast(const Type &result, const Type &operand);
Type checkPostfix(const Type &left, const Type &right);
Type checkFunctionTypes(const std::string name, Parameters &inputParam);
Type checkTest(const Type &testExpr);
Type checkAssignment(const Type &left, const Type &right, const bool isLValue);
Type checkReturn(const Type &right, const Type &funcType);

# endif /* CHECKER_H */
