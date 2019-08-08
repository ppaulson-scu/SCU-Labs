/* Peter Paulson, Tuesday Lab. */


#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_WORD_LENGTH 30

int main(int argc, char *argv[])
{
	int wc = 0;
	char buffer[MAX_WORD_LENGTH];					/* initialize the max possible word length */
	FILE *fp;

	if (argc < 2)							/* if no file is specified, exit */
	{
		printf("Error, file name not specified\n");
		return -1;
	}

	if( access( argv[1], F_OK) != -1){				/* check if the file exists */		

		fp = fopen(argv[1], "r");				/* prep character stream from file */

		while (fscanf(fp, "%s", buffer) != EOF)			/* count words until end of file */
		{
			wc++;
		}
		printf("%d total words\n", wc);				/* print results */
		return 0;
	}else{
		printf("Error, file does not exist\n");			/* if the file doesn't exist, exit */
		return -1;
	}
}
