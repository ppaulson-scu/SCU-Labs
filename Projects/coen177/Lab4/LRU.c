#include "header.h"

#define MAX_MEM_SIZE 500

/*	maxElementIndex(int *arr, int arrlen)
 *
 *	Returns the index of the greatest element of an array.
 *	
 *	@param {int*} arr: the array to be searched
 *	@param {int} arrlen: the length of the array
 *	@return {int} the index of the greatest element of the array
 */

int maxElementIndex(int *arr, int arrlen){
	int max = 0;
	int pos = -1;
	int i;
	for(i = 0; i < arrlen; i++){
		if(arr[i] > max){
			pos = i;
			max = ((arr[i] > max) ? arr[i] : max);
		}
	}
	return pos;
}

int main(int argc, char *argv[]) {
	if ( argc < 2){
		fprintf(stderr, "Improper usage! Provide size of page table as arg.\n");
		return -1;
	}
	
	int tableSize = atoi(argv[1]);

	int pageRequest, pageTableIndex = 0;
	int *pageTable = (int *) malloc(sizeof(int) * MAX_MEM_SIZE);
	int *accessTable = (int *) malloc(sizeof(int) * MAX_MEM_SIZE);
	char *input = NULL;
	ssize_t bytesRead;
	size_t inputAllocated = 0;
	
	while(bytesRead = getline(&input, &inputAllocated, stdin) != EOF) {
		int pageRequest;
		pageRequest = atoi(input);
		if (pageRequest == 0){
			fprintf(stderr, "Improper usage! Provide int page value.\n");
			continue;
		}
		if(!isInMemory(pageRequest, pageTable, tableSize)) {
			if(pageTableIndex < tableSize) {
				pageTable[pageTableIndex] = pageRequest;
				accessTable[pageTableIndex++] = 0;
				printf("Page number %d caused a page fault.\n", pageRequest);
			} else {
				int newIndex = maxElementIndex(accessTable, tableSize);
				pageTable[newIndex] = pageRequest;
				accessTable[newIndex] = 0;
				printf("Page number %d caused a page fault.\n", pageRequest);
			}
		} else {
			int newIndex;
			newIndex = isInMemory(pageRequest, pageTable, tableSize) - 1;
			accessTable[newIndex] = 0;
		}
		int i;
		for(i = 0; i < tableSize; i++){
				accessTable[i]++;
		}
		//printarr(pageTable, tableSize);
		//printarr(accessTable, tableSize);
			
	}

	free(input);
	free(pageTable);
	free(accessTable);
	return 0;
}
