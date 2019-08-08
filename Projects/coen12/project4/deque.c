#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "deque.h"

struct node{
	int data;
	struct node *prev;
	struct node *next;
};

struct deque{
	int count;
	struct node *dummy;
};

//O(1)
DEQUE *createDeque(void){
	
	DEQUE *dp = malloc(sizeof(DEQUE));
	assert(dp != NULL);
	struct node *dummy = malloc(sizeof(struct node));
	assert(dummy != NULL);
	dp->dummy = dummy;
	dp->count = 0;
	dummy->next = dummy;
	dummy->prev = dummy;
	return dp;
	
}
//O(n)
void destroyDeque(DEQUE *dp){
	
	struct node *current = dp->dummy->next;;

	for(int i = 0; i < dp->count; i++)
	{	
		current = current->next;
		free(current->prev);
	}
	free(dp->dummy);
	free(dp);
}

//O(1)
int numItems(DEQUE *dp){
	return dp->count;
}

//O(1)
void addFirst(DEQUE *dp, int x){
	
	struct node *new = malloc(sizeof(struct node));
	assert(new != NULL);
	new->data = x;
	new->next = dp->dummy->next;
	new->prev = dp->dummy;
	dp->dummy->next->prev = new;
	dp->dummy->next = new;	
	dp->count = dp->count + 1;
}

//O(1)
void addLast(DEQUE *dp, int x){
	
	struct node *new = malloc(sizeof(struct node));
	assert(new != NULL);
	new->data = x;
	new->next = dp->dummy;
	new->prev = dp->dummy->prev;
	dp->dummy->prev->next = new;
	dp->dummy->prev = new;
	dp->count = dp->count + 1;
}

//O(1)
int removeFirst(DEQUE *dp){
		
	assert(dp->count !=0);
	int x = dp->dummy->next->data;
	dp->dummy->next->next->prev = dp->dummy;
	dp->dummy->next = dp->dummy->next->next;
	dp->count = dp->count - 1;
	return x;
}

//O(1)
int removeLast(DEQUE *dp){

	assert(dp->count !=0);
	int x = dp->dummy->prev->data;
	dp->dummy->prev->prev->next = dp->dummy;
	dp->dummy->prev = dp->dummy->prev->prev;
	dp->count = dp->count - 1;
	return x;
}

//O(1)
int getFirst(DEQUE *dp){

	assert(dp->count !=0);
	return dp->dummy->next->data;
}

//O(1)
int getLast(DEQUE *dp){
	
	assert(dp->count !=0);
	return dp->dummy->prev->data;
}
