
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <stdbool.h>
#include "deque.h"

#define RADIX 10


struct node{
	int data;
	struct node *prev;
	struct node *next;
};

struct deque{
	int count;
	struct node *dummy;
};

//arg0: 	deque to be sorted
//arg1: 	max element in the deque
//return: 	the sorted deque
DEQUE *radixSort(DEQUE *dp, int x){					
	
	DEQUE arr[9];
	int max = x;
		
	for(int i = 0; i < 10; i++)
	{
		arr[i] = *createDeque();
	}

	int digits = log10(max) + 1;
		
	for(int i = 0; i < digits; i++)					//how many times to process the array
	{
		int totalItems = numItems(dp);
		for(int j = 0; j < totalItems; j++)			//for each element in the main array
		{
			int num = removeFirst(dp);
			int index = (num)/((int)(pow(10, i))) % 10;
			addLast(&arr[index], num);
		}	

		for(int j = 0; j < RADIX; j++)				//for each deque
		{
			int itemsInDeque = numItems(&arr[j]);
			for(int k = 0; k < itemsInDeque; k++)		//for each element
			{
				addLast(dp, removeFirst(&arr[j]));	//move the element to the main list
			}
		}
	}
	return dp;
}

int main(){
	
	DEQUE *main = createDeque();
	assert(main !=NULL);
	int max = 0;
	int x;

	while(scanf("%d", &x) == 1)					//while there are valic integers on sys.in
	{	
		addLast(main, x);
		max = (max < x) ? x : max;
	}
	
	main = radixSort(main, max);					//sort the elements
	
	int elts = numItems(main);
	for(int i = 0; i < elts; i++)					//print the sorted elements
	{
		printf("%d\n", removeFirst(main));
	}
	return -1;
}


