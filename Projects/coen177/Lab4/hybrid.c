#include "header.h"

#define MAX_MEM_SIZE 500

/*	leastRatio(int *freq, int *access, int arrlen)
 *
 *	Returns the index of the element with the least ratio, where ratio is defined as freq[i] / access[i].
 *	
 *	@param {int*} freq: an array of elements that holds the number of times the element has been accessed
 *	@param {int*} access: an array of elements that hold the time since the element was last accessed
 * 	@param {int} arrlen: the length of both arrays
 *	@return {int} the index of the element with the least ratio in the array
 */

int leastRatio(int *freq, int *access, int arrlen){
	float ratio = 1;
	int pos = -1;
	int i;
	for(i = 0; i < arrlen; i++){
		float f = (float)freq[i]/access[i];
		if(f < ratio){
			ratio = f;
			pos = i;
		}
		//fprintf(stderr, "%f ", f);
	}
	//fprintf(stderr, "\n");
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
				int newIndex = leastRatio(freqTable, accessTable, tableSize);
				pageTable[newIndex] = pageRequest;
				accessTable[newIndex] = 0;
				freqTable[newIndex] = 1;
			}
		} else {
			int newIndex;
			newIndex = isInMemory(pageRequest, pageTable, tableSize) - 1;
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
