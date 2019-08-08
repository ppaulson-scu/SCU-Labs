//  Peter Paulson
//  Robert Culkin
//
//
//
//
//
#include <iomanip>
#include "poly.h"
#include <cassert>
#include <climits>
#include <cmath>

namespace coen70_lab4
{
    
    polynomial::polynomial(double c, unsigned int exponent)
    {
        clear();
        current_degree = exponent;
        coef[exponent] = c;
        
    }
    
    void polynomial::update_current_degree()
    {
        for(int i = 0; i <= MAXIMUM_DEGREE; i++)
        {
            if(coef[i] != 0) current_degree = i;
        }
    }
    
    void polynomial::assign_coef(double coefficient, unsigned int exponent)
    {
        assert(exponent <= MAXIMUM_DEGREE);
        coef[exponent] = coefficient;
        update_current_degree();
    }
    
    void polynomial::add_to_coef(double amount, unsigned int exponent)
    {
        assert(exponent <=MAXIMUM_DEGREE);
        coef[exponent] += amount;
        update_current_degree();
    }
    
    void polynomial::clear()
    {
        for(int i = 0; i <= MAXIMUM_DEGREE; i++){
            coef[i] = 0;
        }
        update_current_degree();
    }
    
    polynomial polynomial::antiderivative() const
    {
        assert(degree( ) < MAXIMUM_DEGREE);
        polynomial anti;
        
        for(int i = MAXIMUM_DEGREE; i > 0; i--)
        {
            anti.assign_coef(coef[i-1] / i, i);
        }
        anti.assign_coef(0, 0);
        return anti;
        
    }
    
    double polynomial::coefficient(unsigned int exponent) const
    {
        if(exponent > MAXIMUM_DEGREE) return 0;
        return coef[exponent];
    }
    
    double polynomial::definite_integral(double x0, double x1) const
    {
        polynomial anti= antiderivative();
        return anti.eval(x1) - anti.eval(x0);
    }
    
    polynomial polynomial::derivative() const
    {
        polynomial deriv;
        for(int i = 0; i < MAXIMUM_DEGREE; i++)
        {
            deriv.assign_coef(coef[i+1] * (i + 1), i);
        }
        return deriv;
        
    }
    
    double polynomial::eval(double x) const
    {
        int sum = 0;
        for(int i = 0; i <= MAXIMUM_DEGREE; i++)
        {
            sum += coef[i] * std::pow(x, i);
        }
        return sum;
    }
    
    bool polynomial::is_zero() const
    {
        for(int i = 0; i <= MAXIMUM_DEGREE; i++)
        {
            if (coef[i] != 0) return false;
        }
        return true;
    }
    
    unsigned int polynomial::next_term(unsigned int e) const
    {
        for (unsigned int i = e+1; i <= MAXIMUM_DEGREE; i++)
        {
            if(coef[i] != 0) return i;
        }
        return 0;
    }
    
    unsigned int polynomial::previous_term(unsigned int e) const
    {
        for(int i = e-1; i >=0; i--)
        {
            if(coef[i] != 0) return i;
        }
        return UINT_MAX;
    }
    
    polynomial operator+ (const polynomial &p1, const polynomial &p2)
    {
        polynomial sum;
        for (int i = 0; i <= polynomial::MAXIMUM_DEGREE; i++)
        {
            sum.add_to_coef(p1.coefficient(i), i);
            sum.add_to_coef(p2.coefficient(i), i);
        }
        return sum;
    }
    
    polynomial operator- (const polynomial &p1, const polynomial &p2)
    {
        polynomial dif;
        for (int i = 0; i <= polynomial::MAXIMUM_DEGREE; i++)
        {
            dif.add_to_coef(p1.coefficient(i), i);
            dif.add_to_coef(-p2.coefficient(i), i);
        }
        return dif;
    }
    
    polynomial operator* (const polynomial &p1, const polynomial &p2)
    {
        assert(p1.degree() + p2.degree() <= polynomial::MAXIMUM_DEGREE);
        polynomial product;	
        
        for(int i = 0; i <= p1.degree(); i++)
        {
            for(int j = 0; j <= p2.degree(); j++)
            {
                product.add_to_coef(p1.coefficient(i) * p2.coefficient(j), i+j);
            }
        }
        return product;
    }
    
    std::ostream& operator<< (std::ostream& out, const polynomial &p)
    {
        int count = 0;
        unsigned int i = p.previous_term(polynomial::MAXIMUM_DEGREE+1);
	std::ios_base::fmtflags flags = out.flags();       	

	int prec = out.precision();
	out<< std::setiosflags(std::ios::fixed);
	out.precision(1);
	
 
	if(p.is_zero())
	{
		out << "0";
		return out;
	}
	
	//First term
	if(i > 1)
	{
		++count;
		
        	out << p.coefficient(i) << "x^" << i;
        }
	i = p.previous_term(i);

	while (i != UINT_MAX && i > 1)
        {
		++count;
            if(p.coefficient(i) > 0)
            {
                out << " + ";
            }
            else
            {
                out << " - ";
            }
            out << std::abs(p.coefficient(i)) << "x^" << i;
            i = p.previous_term(i);
        }
        
        //i = 1
        if(i == 1)
        {
            //if(p.coefficient(0) != 0)
	    //{
		++count;
		if(p.coefficient(i) > 0)
            	{
                     out << " + ";
            	}
            	else
            	{
                     out << " - ";
            	}
	    //}
      	out << std::abs(p.coefficient(i)) << "x";
	i = p.previous_term(i);
        	
	}

        
        if(i == 0)
        {
		if(count != 0)
		{	
         	   	if(p.coefficient(i) > 0)
            		{
              	 	 out << " + ";
         	   	}
            		else
            		{
               		 out << " - ";
            		}
		}
            	out << std::abs(p.coefficient(i));
        }
	out.precision(prec);
	out << std::resetiosflags(flags);
        return out;
        
    }
}
