# ifndef CHECKER_H
# define CHECKER_H

# include <string>
# include <vector>
# include <iostream>
# include "Scope.h"

void openScope();
void closeScope();
void declareFunction(std::string ident, Type t);
void defineFunction(std::string ident, Type t);
void declareVariable(std::string ident, Type t);
void checkIdentifier(std::string ident);
void checkFunction(std::string ident);

#endif /* CHECKER_H */
