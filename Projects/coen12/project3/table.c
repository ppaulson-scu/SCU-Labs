#include <stdbool.h>
#include <assert.h>
#include "set.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
		
#define EMPTY 0					//note: 0 = empty, 1 = filled, 2 = removed
#define FILLED 1
#define REMOVED 2

struct set {
	int count;				//the num er of elements in the array
	int length;				//the length of the array
	char **elts;				//the array itself
	int *filled;				//the flag array
};


SET *createSet(int maxElts)			//O(m) runtime
{
	SET *sp;
	sp = malloc(sizeof(SET));
	assert(sp!=NULL);
	sp->count = 0;
	sp->length = maxElts;
	sp->elts = malloc(sizeof(char*)*maxElts);
	sp->filled = malloc(sizeof(int) * maxElts);
	assert(sp->elts!=NULL);
	assert(sp->filled!=NULL);
	for(int i = 0; i < maxElts; i++)	//set each flag to "empty"
	{
		sp->filled[i] = EMPTY;
	}
	return sp;

}

unsigned hashString(char *s)
{
	unsigned hash = 0;
	while (*s != '\0')			//hash function
		hash = 31 * hash + *s++;
	return hash;
}

int findElement(SET *sp, char *elt, bool *found)			//O(1) expected; O(n) worst case
{ 

	unsigned index = hashString(elt)%sp->length;
	int count = 0;
	*found = false;
	int firstEmpty = -1;

	while(sp->filled[index] != EMPTY && count != sp->count)		//check for ending conditions - when an empty slot is reached or the 
	{								//entire array has been searched
		if(sp->filled[index] == REMOVED && firstEmpty < 0)	//save the first removed slot for later
		{
			firstEmpty = index;		
		}

		else if(sp->filled[index] == FILLED && strcmp(sp->elts[index], elt) == 0)	//if the element is found
		{
			*found = true;
			return index;	
		}
		count++;
		index = (index+1) % sp->length;							//increment the index, accounting for overflow
		
	}
	return (firstEmpty > 0) ? firstEmpty : index;						//return the first removed slot from earler if one exists, 
}												//else return the empty slot taht was found



void destroySet(SET *sp)				//O(m) runtime
{
	for(int i = 0; i < sp->count; i++)		//free each elt
	{
		free(sp->elts[i]);
	}
	free(sp->filled);
	free(sp->elts);					//free each elt pointer
	free(sp);					//free arr
}

int numElements(SET *sp)
{
	return sp->count;
}

bool hasElement(SET *sp, char *elt)
{
	bool found;
	int index = findElement(sp, elt, &found);
	return found;	
}

bool addElement(SET *sp, char *elt)			//O(1) + findElement runtime
{
		
	bool hasElt;
	int index = findElement(sp, elt, &hasElt);
	
	if(!hasElt) 					//if the element isn's already in the array
	{
		sp->elts[index] = strdup(elt);
		sp->filled[index] = FILLED;
		sp->count++;
		return true;
	}
	return false;
}

bool removeElement(SET *sp, char *elt)			//O(1) + findElement runtime
{
	bool hasElt;
	int index = findElement(sp, elt, &hasElt);

	if(hasElt)					//if the element is in the array
	{
		sp->elts[index] = NULL;
		sp->filled[index] = 2;
		sp->count--;
		return true;
		
	}
	return false;
}
