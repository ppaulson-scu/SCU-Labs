#include <unistd.h>
#include <ctype.h>
#include "tree.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "pack.h"

	
struct tree{
	int data;
	struct tree *parent;
	struct tree *left;
	struct tree *right;
};


struct tree** leaves;

struct tree** heap;

int count = 0;

//Inserts a new leaf, then percolates.	
//O(logn)
void insert(struct tree* new){
	heap[count] = new;
	assert(heap[count] != NULL);
	
	int index = count++;
	while(getData(heap[index]) < getData(heap[(index-1)/2]) && index != 0){
		struct tree* temp = heap[(index-1)/2];
		heap[(index-1)/2] = heap[index];
		heap[index] = temp;
		index = (index-1)/2;
	}
}

//Deletes and returns the first (minimum) element of the heap, then percolates.
//O(logn)
struct tree* delete(){

	struct tree* deleted = heap[0];
	heap[0] = heap[--count];
	heap[count] = NULL;
	int index = 0;
	while(index < count/2 && heap[2*index+1] != NULL && heap[2*index+2] != NULL && (getData(heap[index]) > getData(heap[2*index+1]) ||getData(heap[index]) > getData(heap[2*index+2]))){
		int balance = getData(heap[index*2+1]) - getData(heap[2*index+2]);
		if(getData(heap[index]) > getData(heap[index*2+1]) && getData(heap[index]) > getData(heap[index*2+2])){
			if(balance <= 0){	
				struct tree* temp = heap[index];
				heap[index] = heap[2*index+1];
				heap[index*2+1] = temp;
				index = index * 2 + 1;
			}else{
				struct tree* temp = heap[index];
				heap[index] = heap[2*index+2];
				heap[index*2+2] = temp;
				index = index * 2 + 2;
			}
		}
		else if(getData(heap[index]) > getData(heap[2*index+1])){
			struct tree* temp = heap[index];
			heap[index] = heap[2*index+1];
			heap[index*2+1] = temp;
			index = index * 2 + 1;
		}
		else if(getData(heap[index]) > getData(heap[2*index+2])){
			struct tree* temp = heap[index];
			heap[index] = heap[2*index+2];
			heap[index*2+2] = temp;
			index = index * 2 + 2;
		}
	}
	
	return deleted;
}

//Prints the binary representation of the leaf's data
//O(logn)
void binary(struct tree* child){

	if(getParent(child) != NULL &&getParent(getParent(child)) != NULL)
		binary(getParent(child));
	if(child == getLeft(getParent(child)))  printf("0"); 	
	if(child == getRight(getParent(child))) printf("1");
}


int main(int argc, char* argv[]){

	int count[257];
	FILE *fp;
	leaves = malloc(sizeof(struct tree*) * 257);
	heap = malloc(sizeof(struct tree*) * 257);
	
	//check if file is accessable
	if(access(argv[1], F_OK) != -1){
		fp = fopen(argv[1], "r");
	}else{
		printf("Error, file inaccessable\n");
		return -1;
	}
	
	//Create leaves by reading file
	for(int i = 0; i < 257; i++)
		count[i] = 0;
	
	//Count characters
	int next= getc(fp);
	while(next != EOF){
		count[next] = count[next] +1;
		next = getc(fp);
	}
	
	//Transfer data from int[] to trees
	for(int i = 0; i < 257; i++){
		if(count[i] != 0)
			leaves[i] = createTree(count[i], NULL, NULL);
	}
	leaves[256] = createTree(0, NULL, NULL);

	//Insert elements into heap
	for(int i = 0; i < 257; i++){
		
		if(leaves[i] != NULL){
			insert(leaves[i]);
		}
	}
	
	
	//Binary Tree construction: delete x2, insert(d1+d2)
	while(heap[1] != NULL){
		struct tree* d1 = delete();
		struct tree* d2 = delete();
		struct tree* new = createTree(getData(d1) + getData(d2), d1, d2);
		d1->parent = new;
		d2->parent = new;
		insert(new);
			
	}
	
	//Pack
	pack(argv[1], argv[2], leaves);
	
	//Print
	for(int i = 0; i < 257; i++){
		//printf("Printing: %d\n", i);
		if(leaves[i] != NULL){
			//printf("leaves[i] != NULL\n");
			if (isprint(i)){  printf("\'%c\' | ", i); }
			else { printf("%03o | ", i); }
			printf("%d | ", getData(leaves[i]));
			binary(leaves[i]);
			printf("\n");
		}
	}
	
	
}

//Notes:
//p(x)  = (x-1)/2
//lc(x) = 2x+1
//rc(x) = 2x+2
//
//delete() - remove and store a[0], move a[n] to a[0], compare with parent until it's 
//insert-(struct tree*) place passed variable into the heap, rebalance
//
//print: 'char' | count | binary coding
//
//
//
