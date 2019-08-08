#include <stdio.h>

int main(int argc, char *argv[]){

	FILE *fptr;
	char c;

	//Setup
	if (argc != 2){
		printf("%d arguments detected, 2 expected.\nValid files:\n	/local/weka/<one_g> | <five_g> | <ten_g>.\n");
		return 0;
	}

	fptr = fopen(argv[1], "r");
	if(fptr == NULL){
		printf("File not found. Valid files:\n	/local/weka/<one_g> | <five_g> | <ten_g>.\n");
		return 0;
	}

	//Process
	do{
		c = fgetc(fptr);
	}while(!feof(fptr));
	
	fclose(fptr);
	return 0;

}
