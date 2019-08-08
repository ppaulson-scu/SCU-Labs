# ifndef PARSER_H
# define PARSER_H

void error();
void match(int c);
int main();

void parse();
void expression();
void and_expression();
void equality_expression();
void comparison_expression();
void add_expression();
void mult_expression();
void unary_expression();
void array_expression();
void primary_expression();
void expression_list();
void specifier();
void pointers();
void statements();
void statement();
void declarations();
void declaration();
void declarator();
void func_or_global();
void global_declarators();
void global_declarator();
void parameters();
void parameter_list();

# endif /* PARSER_H */
