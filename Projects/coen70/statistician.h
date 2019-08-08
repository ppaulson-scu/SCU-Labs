/*
Peter Paulson, Riley Bergin
Santa Clara University

CONSTRUCTORS:
	statistician()
	Postcondition: A statistician exists with no statistics.
	
	statistician(int length, double sum, double last, double largest, double smallest)
	Postcondition: A statistician with the specified statistics.


*/

#ifndef statistician_coen70
#define statistician_coen70

class statistician
{
	int seqlength;
	double seqlast;
	double seqsum;
	double seqsmallest;
	double seqlargest;
	
public:
	statistician();
	statistician(int length, double sum, double last, double largest, double smallest);
	void erase();
	void erase();
	void next_number(double num);
	int length() const {return seqlength;}
	double last() const {return seqlast;}
	double sum() const {return seqsum;}
	double mean() const {return seqsum / seqlength;}
	double smallest() const {return seqsmallest;}
	double largest() const {return seqlargest;}
	void operator=(const statistician &s);
	friend statistician operator+(statistician &s1, statistician &s2);
};
#endif
