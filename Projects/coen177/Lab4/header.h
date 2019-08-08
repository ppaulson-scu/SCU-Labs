#ifndef COEN_177_4
#define COEN_177_4

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/*	isInMemory(int page, int *pageTable, int tableSize)
 *
 *	Checks to see if a page is in memory.
 *	
 *	@param {int} page: the page to be searched
 *	@param {int*} pageTable: the table to be searched for
 *	@param {int} tableSize: the size of the table
 *	@return {int} 0 if the page was not found, else the index of pageTable where page was found plus one. This prevents the first element of the array being the same as the false return value.
 */
int isInMemory(int page, int *pageTable, int tableSize) {
	int i;
	for (i = 0; i < tableSize; i++){
		if(pageTable[i] == page) {
			return i + 1;
		}
	}
	return 0;
} 

/*	printarr(int *arr, int arrlen)
 *
 *	Utility function to print an array.
 *	
 *	@param {int*} arr: the array to be printed
 *	@param {int} arrlen: the number of elements to be printed
 *	@return {int} 0.
 */
int printarr(int *arr, int arrlen){
	int i;
	for(i = 0; i < arrlen; i++){
		fprintf(stderr, "%d ", arr[i]);
	}
	fprintf(stderr, "\n");
	return 0;
}

#endif
