# include <iostream>
# include <cassert>
# include "Type.h"
# include "tokens.h"

using namespace std;

void print(Type t){
	cout << t << endl;
}

int main(){
	
	Type t(INT);
	print(t);
	print(t.promote());
	
	Type c(CHAR);
	print(c);
	print(c.promote());

	cout << c.isCompatibleWith(t) << endl;
	cout << t.isCompatibleWith(c) << endl;

	Type arr(DOUBLE, 0, 10);
	print(arr);
	print(arr.promote());
	cout << arr.isNumeric() << endl;
	cout << c.isNumeric() << endl;
	Type charptr(CHAR, 2);
	Type floatptr(CHAR, 2);

	cout << floatptr.isCompatibleWith(charptr) << endl;
	cout << charptr.isCompatibleWith(floatptr) << endl;

	

}
