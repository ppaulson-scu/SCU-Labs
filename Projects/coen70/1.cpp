/*
Authors: Peter Paulson, Zachary Bellay
*/
#include <iomanip>
#include <cstdlib>
#include <iostream>
#include <string>

void printNums()
{
	std::string ascend = "0123456789";
	std::string descend = "9876543210";
	
	for(int i = 0; i < 10; i++)	
	{
		std::string str;
		if (i%2 == 0)			
		{
			str = ascend;		//on even lines, print ascending string
		}else{
			str = descend;		//on odd lines, print descending string
		}
 
		std::cout << std::setw(i + str.length()) << str << "\n";	//print the string and assign the correct offset using setw()
	}

}



int main()
{
	printNums();
	return 0;
}
