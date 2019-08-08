// Test File 

#include "sequence.h"
#include <iostream>
using namespace coen70_lab3;

void print_sequence(const sequence &s);

int main(){

	sequence s;
	std::cout << s.size() << std::endl;
	s.insert(10);
	std::cout <<"1="<< s.is_item() << std::endl;
	print_sequence(s);
	s.insert(15);
	print_sequence(s);
	s.insert(25);
	print_sequence(s);
	for(int i = 30; i <=  50; i+=5){
		s.insert(i);
		print_sequence(s);
	}
	for(int i = 3; i <= 27; i+=3){
		s.attach(i);
		s.advance();
		print_sequence(s);
	}
	s.remove_current();
	print_sequence(s);
	s.remove_front();
	print_sequence(s);
	s.attach_back(911);
	print_sequence(s);
	s.insert_front(4411);
	std::cout << std::endl;
	print_sequence(s);
	sequence r;
	for(double i = 3.1415; i < 100; i = i*2)
	{
		r.attach(i);
	}
	print_sequence(r);
	std::cout << std::endl;
	print_sequence(s + r);
	std::cout << (s+r).size() << std::endl;
	s+=r;
	print_sequence(s);
	return EXIT_SUCCESS;
}

void print_sequence(const sequence &s) {

	std::cout << "[";
	for(int i = 0; i < s.size(); i++){
		std::cout << s[i] << " ";
	}
	std::cout << "]" << std::endl;
}
