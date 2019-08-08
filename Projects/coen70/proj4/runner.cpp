//Runner Test class for the poly class.
//Peter Paulson

#include "poly.h"
#include <iostream>

using namespace coen70_lab4;

int main()
{
	//polynomial p3(10, 10);
	polynomial p1(5, 1);
	polynomial p2(5, 1);
	

	p1.assign_coef(2, 0);
	
	p2.assign_coef(2, 0);
	p2.add_to_coef(9.11, 2);
	p2.add_to_coef(-6.3, 4);
	p2.add_to_coef(-4, 15);
	
	p1.add_to_coef(2.8, 1);
	p1.add_to_coef(-1.22, 2);
	p1.add_to_coef(3.3365, 4);
	
	for(int i = 0; i <=12; i++)
	{
		p1.add_to_coef(i/10., i);
	}
	std::cout << p2 << std::endl;
	std::cout << "Antiderivative" << std::endl << p2.antiderivative() << std::endl;
	std::cout << "Definite Integral" << std::endl << p2.definite_integral(0, 1) << std::endl;
	std::cout << "Derivative" << std::endl << p2.derivative() << std::endl;
	std::cout << "Eval at 1: " << p2.eval(1) << std::endl << std::endl << std::endl;
	std::cout << p2 << std::endl;
	std::cout << p1 << std::endl;
	std::cout << std::endl << p1 + p2 << std::endl;
	std::cout << p1 - p2 << std::endl;
	std::cout << p2 - p1 << std::endl;
	
	p1.clear();
	p2.clear();
	for(int i = 0; i < 2; i++)
	{
		p1.assign_coef(1, i);
		p2.assign_coef(1, i);
	}	
	
	std::cout << p1 << std::endl << p2 << std::endl;

	std::cout << p1 * p2 << std::endl;	


	polynomial p;
	for(int i = 0; i <=29; i++)
	{
		p.assign_coef(i%2==1 ? i / 1.2 : -i/17.0, i);
		std::cout << p << std::endl;
	}
}
