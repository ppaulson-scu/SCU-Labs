//
//  mystring.cpp
//  
//  Peter Paulson, Asa Jacob
//
//  Invariant:
//      The dynamic character array sequence contains the characters of the represented string ordered by index.
//      The size_t allocated represents number of characters able to be held in the dynamic array sequence.
//      The size_t current_length represents the current number of characters in the string that is being represented.

#include "mystring.h"
#include <stdio.h>
#include <cassert>
#include <cstring>
#include <cctype>
#include <algorithm>

namespace coen70_lab5
{
    string::string(const char str[ ])
    {
        current_length = strlen(str);
        sequence = new char[current_length+1];      //Allocate sequence as a dynamic array
        reserve(current_length+1);
        strncpy(sequence, str, current_length+1);

    }    
    
    string::string(const string& source)
    {
        sequence = new char[source.allocated];
        allocated = source.allocated;
        current_length = source.current_length;
        std::copy(source.sequence, source.sequence + current_length, sequence);
    }
    
    string::~string()
    {
        //std::cout << "Destructor: " <<  sequence << std::endl;
        delete [ ] sequence;
    }
    
    void string::reserve(size_t n)
    {
        if(n == allocated)          //Check to see if the requested amount is the current amoount
            return;
        if(n < allocated)           //Check to see if the current sequence can be held in the requested allocation.
            n = current_length;
        
        
        
        char *newseq = new char[current_length+1];
        std::copy(sequence, sequence+current_length, newseq);
        delete [ ] sequence;
        sequence = newseq;
        allocated = n;
    }
    
    void string::operator +=(const string& addend)
    {
        if(current_length + addend.length() > allocated) reserve(current_length + addend.length() + 1);
        std::strncat(sequence, addend.sequence, addend.length());
    }
    void string::operator +=(const char addend[ ])
    {
        string newstr(addend);
        *this+=newstr;
    }
    void string::operator +=(char addend)
    {
        char newchar[2];
        newchar[0] = addend;
        newchar[1] = '\0';
        *this+=newchar;
    }
    
    char string::operator [ ](size_t position) const
    {
        assert(position < length());
        return sequence[position];
    }
    
    string& string::operator =(const string& source)
    {
        if(this == &source) return *this;
        delete [ ] sequence;
        char *newseq = new char[source.allocated];
        current_length = source.current_length;
        reserve(source.allocated);
        strncpy(newseq, source.sequence, current_length+1);
        sequence = newseq;
        return *this;
    }
    
    std::ostream& operator <<(std::ostream& outs, const string& source)
    {
        outs << source.sequence;
        return outs;
    }
    
    bool operator ==(const string& s1, const string& s2)
    {
        if(s1.length() != s2.length()) return false;
        return strncmp(s1.sequence, s2.sequence, s1.length()) == 0;
    }
    
    bool operator !=(const string& s1, const string& s2)
    {
        return !(s1 == s2);
    }
    
    bool operator > (const string& s1, const string& s2)
    {
        return strncmp(s1.sequence, s2.sequence, s1.length()) > 0;
    }
    
    bool operator < (const string& s1, const string& s2)
    {
        return strncmp(s1.sequence, s2.sequence, s1.length()) < 0;
    }
    
    bool operator >=(const string& s1, const string& s2)
    {
        return !(s1 < s2);
    }
    
    bool operator <=(const string& s1, const string& s2)
    {
        return !(s1 > s2);
    }
    
    string operator +(const string& s1, const string& s2)
    {
        string s3(s1);
        s3+=s2;
        return s3;
    }
    
    string operator+(const string& s1, const char addend[ ])
    {
        string s3(s1);
        string s4(addend);
//        std::cout << s1 << std::endl;
//        std::cout << s4 << std::endl;
        
        s3+=s4;
        return s3;
    }
    
    std::istream& operator >> (std::istream& ins, string& target)
    {
     	 
        while(ins && isspace(ins.peek()))
              ins.ignore();
            
        while(ins && !isspace(ins.peek()))
              target+= ins.get();
        return ins;
    }
}
