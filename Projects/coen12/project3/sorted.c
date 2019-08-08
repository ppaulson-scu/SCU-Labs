#include <stdbool.h>
#include <assert.h>
#include "set.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

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

int findElement(SET *sp, char *elt, bool *found)
{
        
        int lo = 0;
        int hi = sp->count-1;
        int mid = (lo+hi)/2;				
        while(lo<=hi)					//while elt is between 2 other elements					
        {
                mid = (hi+lo)/2;
                int cmp = strcmp(sp->elts[mid], elt);
                
                if(cmp < 0)				//if elt comes after elts
                {
                        lo = mid+1;
                }
                else if(cmp > 0)			// if elt comes before elts
                {
                        hi = mid-1;
                }
		else					//if elt == elts
		{
			*found = true;

			return mid;
		}

		
        }

	*found = false;		
	return mid;
        
}



void destroySet(SET *sp)
{
	for(int i = 0; i < sp->count; i++)		//free each elt
	{
		free(sp->elts[i]);
	}
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

bool addElement(SET *sp, char *elt)
{
	
	bool hasElt; 
	int index = findElement(sp, elt, &hasElt);

	if(!hasElt)						// if the arr does not already contain the element
	{	

		elt = strdup(elt);
		for(int i = sp->count-1; i >= index; i--)	//for each element between the insertion index and the end of the array		
		{
			sp->elts[i+1] = sp->elts[i];
		}
		sp->elts[index] = elt;
		sp->count++;

		return true;
	}
	return false;
}

bool removeElement(SET *sp, char *elt)
{
	bool found;
	int index = findElement(sp, elt, &found);
	if (!found) return false;
	
	for(int i = index; i<sp->count-1; i++)			//fopr each element between the deletion index and the end of the array
	{
		sp->elts[i]=sp->elts[i+1];
	}
	
//	free(sp->elts[sp->count-1]);
	sp->count--;
	return true;
}
