/*Peter Paulson, Tuesday lab */

#include <stdbool.h>
#include <assert.h>
#include "set.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


struct set {
	int count;
	int length;
	char **elts;
};


SET *createSet(int maxElts)
{
	SET *sp;
	sp = malloc(sizeof(SET));
	assert(sp!=NULL);
	sp->count = 0;
	sp->length = maxElts;
	sp->elts = malloc(sizeof(char*)*maxElts);
	assert(sp->elts!=NULL);
	return sp;
}


int findElement(SET *sp, char *elt)
{	
	for(int i = 0; i < sp->count; i++)	//for each element
	{
		if(strcmp(elt, sp->elts[i])==0)	//if the elements match
		{
			return i;
		}	
	}
	return -1;
}

void destroySet(SET *sp)
{
	for(int i = 0; i < sp->count; i++) //for each element
	{
		free(sp->elts[i]);
	}
	free(sp->elts);
	free(sp);
}

int numElements(SET *sp)
{
	return sp->count;
}

bool hasElement(SET *sp, char *elt)
{
	return (findElement(sp, elt) >= 0);
}

bool addElement(SET *sp, char *elt)
{
	if(!hasElement(sp, elt))		//if the element is not already in the array
	{
		elt = strdup(elt);
		sp->elts[sp->count]=elt;
		sp->count++;
		return true;
	}
	return false;
}

bool removeElement(SET *sp, char *elt)
{
	int index = findElement(sp, elt);
	if (index == -1) return false;
	
	for(int i = index; i<sp->count-1; i++) //for each element from the end of the array to the removal index
	{
		sp->elts[i]=sp->elts[i+1];
	}


//	free(sp->elts[sp->count-1]);
	sp->count--;
	return true;
	
}
