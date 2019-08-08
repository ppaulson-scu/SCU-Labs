#include "header.h"

#define MAX_MEM_SIZE 500

int main(int argc, char *argv[]) {
	if ( argc < 2){
		fprintf(stderr, "Improper usage! Provide size of page table as arg.\n");
		return -1;
	}
	
	int tableSize = atoi(argv[1]);

	int pageRequest, pageTableIndex = 0;
	int *pageTable = (int *) malloc(sizeof(int) * MAX_MEM_SIZE);
	char *input = NULL;
	ssize_t bytesRead;
	size_t inputAllocated = 0;
	
	while(bytesRead = getline(&input, &inputAllocated, stdin) != EOF) {
		int pageRequest;
		pageRequest = atoi(input);
		if (pageRequest == 0){
			fprintf(stderr, "Improper usage! Provide int page value.\n");
		}
		if(!isInMemory(pageRequest, pageTable, tableSize)) {
			printf("Page number %d caused a page fault.\n", pageRequest);
			if(pageTableIndex < tableSize) {
				pageTable[pageTableIndex++] = pageRequest;
			} else {
				pageTableIndex = 0;
				pageTable[pageTableIndex++] = pageRequest;
			}
		}
	}

	free(input);
	free(pageTable);
	return 0;
}
