/*
Peter Paulson, Zachary Bellay
*/
#include <fstream>
#include <iostream>
#include <string>
#include <ctype.h>

int main(int argc, char* argv[1])
{
	std::ifstream inputFile(argv[1]);	//Open file
	std::string currentWord;		//Word to be operated on
	while (inputFile >> currentWord)	
	{
		if(currentWord.length() > 9)					//check if the word should be printed
		{
			for(int i = 0; i < currentWord.length(); i++)		//for each character in the word, change it to uppercase, then print it
			{	
				char chr = std::toupper(currentWord.at(i));	
				std::cout << chr;
			}
			std::cout << "\n";
		}
	}
	return 0;
}
