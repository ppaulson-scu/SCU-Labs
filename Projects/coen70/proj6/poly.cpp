//Peter Paulson, Manoj Adhikari
//Coen 70, Lab 6
//10-30-2016

#include <iostream>
#include <iomanip>
#include <cassert>
#include <cmath>
#include <climits>
#include "poly.h"

using namespace std;
/*
 Invariants: 
 1. coef_size equals the number of items in the sequence pointed by *coef.
 2. If *coef is NULL, coef_size is 0
 3. current_degree is always the highest degree of the polynomial.
 4. current_degree can never be greater that coef_size
 */

namespace coen70_lab6
{
    	// CONSTANTS
    //const unsigned int polynomial::MAXIMUM_DEGREE;

    // HELPER FUNCTIONS
    /*
     This function sets the current degree to the highest exponent
     with a non-zero coeficient.
     */
    void polynomial::update_current_degree() {
        current_degree = 0;

        for (int i = coef_size; i > 0; i--) {
            if (coef[i] != 0.0)
            {
                current_degree = i;
                break;
            }
        }
    }
    /*
     This function checks to see if the exponent is greater than the current size of the coefficient array,
     If it is, the array's size is increased
     */
    void polynomial::reallocateForExponent(unsigned int exponent){
    
        if (exponent+1 == coef_size)
            return;
        if (exponent<current_degree)
            exponent = current_degree;
        double* new_seq = new double[exponent+1];
        for(int i=0;i<=exponent;i++)
            new_seq[i]=0.0;
        //Copy values from coef to sequence so that we can delete coef and resign it to the bigger new_seq.
        std::copy(coef,coef+current_degree+1,new_seq);
        delete[] coef;
        coef=new_seq;
        coef_size=exponent;
    }

    // CONSTRUCTOR
    /*
     A polynomial is created with all zero coefficients, except for coefficient specified in the exponent.
     If no value is received, default value of 0 is assigned to all coefficients.
     */
	polynomial::polynomial(double c, unsigned int exponent){
        
        coef = new double[exponent+1];
        
        current_degree=exponent;
        
        for (size_t i = 0; i <= current_degree; i++) {
            if (i == exponent)
                coef[i]= c;
            else
                coef[i] = 0.0;
        }

        coef_size=current_degree;
        
    }
    
    //COPY CONSTRUCTOR
    /*
     The polynomial in the argument is copied to this polynomial
     */
    polynomial::polynomial(const polynomial& other){
        
        coef = new double[other.coef_size];
        current_degree=other.current_degree;
        coef_size = other.coef_size;
        std::copy(other.coef, other.coef + coef_size, coef);
        
    }

    
    
    //DESTRUCTOR
    //The polynomial gets deleted
    polynomial::~polynomial(){
    
        delete[] coef;
    }

	// MODIFICATION MEMBER FUNCTIONS
    /*
     This function sets the required coefficient for the given exponent
     */
	void polynomial::assign_coef(double coefficient, unsigned int exponent) {
        if(exponent>current_degree)
            reallocateForExponent(exponent);
            
        coef[exponent] = coefficient;
        //current_degree is updated if needed
        update_current_degree();
    }

    /*
     This function adds the given amount to the coefficient of the 
     specified exponent.
     */
    void polynomial::add_to_coef(double amount, unsigned int exponent){
        if(exponent>current_degree)
            reallocateForExponent(exponent);

        // Access, add, and assign.
        assign_coef(coefficient(exponent) + amount, exponent);
    }

    /*
     This function sets all the coefficients of this polynomial to zero
     */
	void polynomial::clear( ) {
        // Zero out all the coeficients.
        for (size_t i = 0; i <= current_degree; i++) {
            coef[i] = 0.0;
        }

        current_degree = 0;
    }

	// // CONSTANT MEMBER FUNCTIONS
    /*
     This function returns the antiderivative of this polynomial
     */
    polynomial polynomial::antiderivative( ) const {

		polynomial p;
  

		for(int i = degree()+1; i > 0; i--)
		{
			p.assign_coef(coefficient(i-1) / i, i);
		}
		p.assign_coef(0, 0);
        
	return p;
    }

    /*
     This function returns the coefficient that matches the given exponent
     */
	double polynomial::coefficient(unsigned int exponent) const {
        // Always return 0.0 if exponent is greater than maximum degree.
        if (exponent > current_degree)
            return 0.0;

        return coef[exponent];
    }

    /*
     This function computes definite integral from x0 to x1 and returns the value
     */
    double polynomial::definite_integral(double x0, double x1) const {
        polynomial anti = this->antiderivative();

        return anti.eval(x1) - anti.eval(x0);
    }

    /*
     This function computes and returns the first derivative of this polynomial
     */
	polynomial polynomial::derivative( ) const {
        polynomial p;

        // Calculate derivative.
        for (size_t i = 0; i < this->degree(); i++) {
            p.assign_coef( this->coefficient(i+1) * (i+1) , i);
        }

        return p;
    }

    /*
     This function evaluates this polynomial when it has the value equal to x assigned to it
     */
	double polynomial::eval(double x) const {
        double sum = 0;
        // Evaluation loop.
        for (size_t i = 0; i <= current_degree; i++) {
            double cur_coef = coefficient(i);
            if (cur_coef != 0) {
                sum += cur_coef * pow(x, i);
            }
        }

        return sum;
    }

    /*
     Returns true only if this polyonmial is the zero polynomial
     */
    bool polynomial::is_zero( ) const {
        if (current_degree == 0 && coef[0] == 0.0)
            return true;
        else
            return false;
    }

    /*
     The first exponent greater than e in the polynomial is returned. If there is no 
     such exponent, 0 is returned.
     */
	unsigned int polynomial::next_term(unsigned int e) const {
        // Search for next term.
        for (size_t i = e+1; i <= current_degree; i++) {
            if (coef[i] != 0)
                return i;
        }
        return 0;
    }

    /*
     The first exponent smaller than e in the polynomial is returned. If there is no
     such exponent, 0 is returned.
     */
    // Search for previous term.
	unsigned int polynomial::previous_term(unsigned int e) const {
        for (int i = e-1; i >= 0; i--) {
            if (coef[i] != 0)
                return i;
        }
        return UINT_MAX;
    }

    // // NON-MEMBER BINARY OPERATORS
    /*
     The return-value is a polynomial with each coefficient equal to the sum of the coefficients 
     of p1 & p2 for any given exponent.
     */
    polynomial operator +(const polynomial& p1, const polynomial& p2)
    {
        polynomial p3;
        for (size_t i = 0; i <= p2.degree(); i++) {
            p3.add_to_coef(p2.coefficient(i), i);
        }
	
        for (size_t i = 0; i <= p1.degree(); i++) {
            p3.add_to_coef(p1.coefficient(i), i);
        }

        return p3;
    }

    /*
     The return-value is a polynomial with each coefficient equal to the difference 
     of the coefficients of p1 & p2 for any given exponent.
     */
    polynomial operator -(const polynomial& p1, const polynomial& p2) {
        polynomial p3;
        for (size_t i = 0; i <= p2.degree(); i++) {
            p3.add_to_coef(-p2.coefficient(i), i);
        }
	
        for (size_t i = 0; i <= p1.degree(); i++) {
            p3.add_to_coef(-p1.coefficient(i), i);
        }
        return p3;
    }
    
    /*
     Each term of p1 has been multiplied by each term of p2,
     and the answer is the sum of all these term-by-term products.
     */
    polynomial operator *(const polynomial& p1, const polynomial& p2) {
        polynomial p3;
        for (size_t i = 0; i <= p1.degree(); i++) {
            for (size_t j = 0; j <= p2.degree(); j++) {
                p3.add_to_coef(p1.coefficient(i) * p2.coefficient(j), i+j);
            }
        }

        return p3;
    }

    // NON-MEMBER OUTPUT FUNCTIONS
    /*
     The polynomial has been printed to ostream out, which, in turn,
     has been returned to the calling function.
     */
    std::ostream& operator << (std::ostream& out, const polynomial& p)
    {
        // Save output settings.
        long savedFlags = out.ios::flags();
        int pres = out.precision();
        out << std::fixed;
        out << std::setprecision(1);

        if (p.is_zero())
            out << 0.00;
        else
        {
            bool firstPrint = true;
            //  Loop through array.
            for (int i = p.degree(); i >= 0; i--) {
                double theCoef = p.coefficient(i);
                if (theCoef != 0) {
                    // Don't print a plus for first coefficient.
                    if (!firstPrint) {
                        out << ((theCoef < 0) ? " - " : " + ");
                    }

                    out << ((firstPrint) ? theCoef : abs(theCoef));
                    // No need to print x^0
                    if (i > 0)
                    {
                        out << "x";
                        // No need to print ^1
                        if (i > 1)
                            out << "^" << i;
                    }

                    if (firstPrint)
                        firstPrint = false;
                }
            }
        }

        // Restore output settings.
        out << setprecision(pres);
        out.flags(savedFlags);
        return out;
    }
}
