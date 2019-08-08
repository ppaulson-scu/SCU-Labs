#include "mystring.h"


using namespace coen70_lab6;
using std::cout;
using std::endl;

int main() 
{
	string s("abc");
	cout << s << endl;
	s+= "def";
	cout << s.count('a') << endl;
	string s1('c');
	cout << s1 << endl;

	string alphabet("abcdefghijklmnopqrstuvwxyz");
	cout << alphabet << endl;

	alphabet.insert("123456", 1);
	cout << alphabet << endl;

	alphabet.dlt(1, 6);
	cout << alphabet << endl;	

	alphabet.replace('a', 1);
	cout << alphabet << endl;

	alphabet.replace("bb", 0);
	cout << alphabet << endl;

	cout << alphabet.search('x') << endl;

	cout << alphabet.search('a') << endl;

	cout << alphabet.count('b') << endl;
	
	cout << alphabet.search("xyz") << endl;

	return EXIT_SUCCESS;
}
