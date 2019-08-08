/* 
Test class for statistitian class
Peter Paulson
*/
#include <iostream>
#include "statistician.h"

using namespace std;

int main()
{
	statistician stat, stat2, stat3, stat4;
	stat.next_number(10);
	stat.next_number(-17.411);
	stat.next_number(1.669);
	cout << "Mean = " << stat.mean() << endl;
	cout << "Length = " << stat.length() << endl;
	cout << "Highest = " << stat.largest() << endl;
	cout << "Lowest = " << stat.smallest() << endl;
	cout << "Sum = " << stat.sum() << endl;
	
	cout << endl;
	stat2.next_number(-2.5);
	stat2.next_number(122);
	stat2.next_number(-18.9999);
	
	stat3 = stat + stat2;
	
	cout << "Mean = " << stat3.mean() << endl;
	cout << "Length = " << stat3.length() << endl;
	cout << "Highest = " << stat3.largest() << endl;
	cout << "Lowest = " << stat3.smallest() << endl;
	cout << "Sum = " << stat3.sum() << endl;
	
	cout << endl << stat4.mean() << endl;
	return EXIT_SUCCESS;
}
