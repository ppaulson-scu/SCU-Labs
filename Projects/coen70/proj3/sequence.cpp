/*
Peter Paulson, Daisuke Kurita
Aff. Santa Clara University
October 2016
*/



#include "sequence.h"
#include <cassert>

using namespace coen70_lab3;

sequence::sequence()
{
	used = 0;		//Set values for
	current_index = 0;	//an empty sequence
}

void sequence::start()
{
	current_index = 0;
}

void sequence::advance()
{
	assert(is_item());	//make sure we don't run too far off the end of the sequence.
	current_index++;
}

void sequence::insert_front(const value_type& entry)
{
	assert(size() < CAPACITY);		//Is this going to fit?
	for (int i = used-1; i >= 0; i--)	//Start at the lst element, go to the first
		{
			data[i+1] = data[i];	//Move them all 1 position up
		}
	data[0] = entry;			//Insert new element at position 0
	++used;
	current_index = 0;
}

void sequence::remove_front()
{
	assert(used > 0);
	for(int i = 0; i < used-1; i++)		//Start at the front, go to the end 
		{
			data[i] = data[i+1];	//Move everything 1 position down. 
		}
	data[used-1] = 0;			//Remove the duplicate last item.
	++used;					//Don't have to manually remove the first item, since it gets overwritten
	current_index = 0;
}
void sequence::attach_back(const value_type& entry)
{
	assert(size() < CAPACITY);	
	data[used] = entry;			
	current_index = used-1;
	++used;
}

void sequence::end()
{	
	if(used == 0) return;
	current_index = used-1;
}

void sequence::insert(const value_type& entry)
{
	assert(size() < CAPACITY);	
	if(!is_item()){ insert_front(entry); }			//Check for the 2nd case of no item to insert behind
	else 
	{
		for(int i = used-1; i != current_index-1; i--)	//Move from the end of the array to just before the element we're inserting after.
		{						//Note that this is != instead of >= because >= didn't behave as expected and != did.
			data[i+1] = data[i];
		}
		data[current_index] = entry;
		++used;
	}	
}

void sequence::attach(const value_type& entry)
{
	assert(size() < CAPACITY);
	if(!is_item()){ attach_back(entry); }			//Check for the 2nd case of no item to insert after.
	else
	{
		for(int i = used; i > current_index; i--)	//Move from the end of the array until the element before the current one
		{
			data[i] = data[i-1];
		}
		data[current_index+1] = entry;
		used++;
		current_index++;
	}
}

void sequence::remove_current()
{	
	assert(is_item());
	for(int i = current_index; i < used-1; i++) 	//Move from the to-be-removed element to the end of the array.
	{
		data[i] = data[i+1];
	}
	data[used-1] = 0;				//Clean up the end of the array.
	used--;
} 

coen70_lab3::sequence::size_type sequence::size() const
{
	return used;
}

bool sequence::is_item() const
{
	return current_index < used;
}

coen70_lab3::sequence::value_type sequence::current() const
{
	assert(is_item());
	return data[current_index];
}

sequence sequence::operator+(const sequence &s)
{
	sequence seq;
	int a, b;
	a = used;
	b = s.size();
	std::cout << b << std::endl;
	for(int i = 0; i < a; i++)
	{
		seq.attach_back(data[i]);
	}
	for(int i = 0; i < b; i++)
	{
		seq.attach_back(s[i]);
	}
	return seq;
}

void sequence::operator+=(const sequence &s)
{
	for(int i = 0; i < s.size(); i++)
		attach_back(s[i]);
}

coen70_lab3::sequence::value_type sequence::operator[](size_type index) const
{
	assert(index < used);
	return data[index];
}


