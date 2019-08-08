#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>

/*
//A simple shell program.
//Behavior: Exits when "exit" is input.
//OTherwise, attempt to run the input, effectively inputting "./<input>" into Bash.
*/
int main(){	

	char command[128];

	while(1){
		scanf("%s", command);
		if(strcmp(command, "exit") == 0) return 0;
	
		if(fork() == 0){
			execve(command, NULL, 0);
			break;
		}else{
			int status;
			waitpid(-1, &status, 0);
		}
	}
}
