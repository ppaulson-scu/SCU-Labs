/*
Peter Paulson, Riley Bergin
Santa Clara University
*/

#include "statistician.h"
#include <limits>
using namespace std;


statistician::statistician()
{
	seqlength = 0;
	seqsum = 0;	
}

statistician::statistician(int length, double sum, double last, double largest, double smallest)
{
	seqlength = length;
	seqsum = sum;
	seqlast = last;
	seqlargest = largest;
	seqsmallest = smallest;
}

void statistician::next_number(double num)
{
	if (seqlength == 0) seqlargest = seqsmallest = num; 
	seqlength++;
	seqsum+=num;
	seqlast = num;
	if(num > seqlargest) seqlargest = num;
	if(num < seqsmallest) seqsmallest = num;
}

void statistician::erase()
{
	seqlength = 0;
	seqsum = 0;
	seqlargest = 0;
	seqsmallest = 0;
	seqlast = 0;
}

statistician operator+(statistician &s1, statistician &s2)
{
	int newLength = s1.length() + s2.length();
	double newSum = s1.sum() + s2.sum();
	double newLargest = (s1.largest() > s2.largest()) ? s1.largest() : s2.largest();
	double newSmallest = (s1.smallest() < s2.smallest()) ? s1.smallest() : s2.smallest();
	double newLast = s2.last();
	return statistician(newLength, newSum, newLast, newLargest, newSmallest);
}

void statistician::operator =(const statistician &s)
{
	this->seqlength = s.length();
	this->seqsum = s.sum();
	this->seqsmallest = s.smallest();
	this->seqlargest = s.largest();
	this->seqlast = s.last();
}
