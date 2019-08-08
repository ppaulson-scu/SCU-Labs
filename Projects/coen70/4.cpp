struct object{
	string* s;
};
object A, B;
A.s = new string;
*A.s = "Hello World"; 
B = A;
*A.s = "Goodbye";
cout << *A.s << endl;
cout << *B.s << endl;
