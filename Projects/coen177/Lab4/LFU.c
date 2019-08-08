#include "header.h"

#define MAX_MEM_SIZE 500

/*	minElementIndex(int *arr, int *access, int arrlen)
 *
 *	Returns the index of the least element of an array. This function uses
 *	the time since last access of the element, found in access, as a tiebreaker.
 *	
 *	@param {int*} freq: the array to be searched
 *	@param {int*} access: an array representing the time since each element in freq was accessed
 *	@param {int} arrlen: the length of the array
 *	@return {int} the index of the least element of the array
 */

int minElementIndex(int *freq, int *access, int arrlen){
	int min = INT_MAX;
	int pos = -1;
	int i;
	for(i = 0; i < arrlen; i++){
		if(freq[i] < min){
			pos = i;
			min = freq[i];
		} else if(freq[i] == min){
			if(access[i] > access[pos]){
				pos = i;
			}
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
	int *freqTable = (int *) malloc(sizeof(int) * MAX_MEM_SIZE);
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
			printf("Page number %d caused a page fault.\n", pageRequest);
			if(pageTableIndex < tableSize) {
				pageTable[pageTableIndex] = pageRequest;
				accessTable[pageTableIndex] = 0;
				freqTable[pageTableIndex++] = 1;
			} else {
				int newIndex = minElementIndex(freqTable, accessTable, tableSize);
				pageTable[newIndex] = pageRequest;
				accessTable[newIndex] = 0;
				freqTable[newIndex] = 1;
			}
		} else {
			int newIndex;
			newIndex = isInMemory(pageRequest, pageTable, tableSize) - 1;
			accessTable[newIndex] = 0;
			freqTable[newIndex]++;
			
		}
		int i;
		for(i = 0; i < tableSize; i++){
				accessTable[i]++;
		}
		//printarr(pageTable, tableSize);
		//printarr(accessTable, tableSize);
		//printarr(freqTable, tableSize);
			
	}

	free(input);
	free(pageTable);
	free(accessTable);
	free(freqTable);
	return 0;
}
