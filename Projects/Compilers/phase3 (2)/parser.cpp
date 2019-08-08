static void primaryExpression(bool lparenMatched)
{
    if (lparenMatched) {
	expression();
	match(')');

    } else if (lookahead == STRING) {
	match(STRING);

    } else if (lookahead == INTEGER) {
	match(INTEGER);

    } else if (lookahead == REAL) {
	match(REAL);

    } else if (lookahead == ID) {
	string name = identifier();

	if (lookahead == '(') {
	    match('(');

	    if (lookahead != ')') {
		expression();

		while (lookahead == ',') {
		    match(',');
		    expression();
		}
	    }

	    match(')');
	    checkFunction(name);

	} else
	    checkIdentifier(name);

    } else
	error();
}

