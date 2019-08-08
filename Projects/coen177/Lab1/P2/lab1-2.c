#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

//A program that creates 8 processes in addition to the original process.
//Design: each process creates a single child, until we have 8 parent-child processes
//Input: n/a
//Output: Each child prints its PID and its parent's PID.
int main(){
	
	int stat;

	if(fork() == 0){
		printf("My PID is %d, my parent's PID is %d\n", getpid(), getppid());
		if(fork() == 0){	
			printf("My PID is %d, my parent's PID is %d\n", getpid(), getppid());
			if(fork() == 0){
				printf("My PID is %d, my parent's PID is %d\n", getpid(), getppid());
				if(fork() == 0){
					printf("My PID is %d, my parent's PID is %d\n", getpid(), getppid());
					if(fork() == 0){
						printf("My PID is %d, my parent's PID is %d\n", getpid(), getppid());
						if(fork() == 0){
							printf("My PID is %d, my parent's PID is %d\n", getpid(), getppid());
							if(fork() == 0){
								printf("My PID is %d, my parent's PID is %d\n", getpid(), getppid());
								if(fork() == 0){
									printf("My PID is %d, my parent's PID is %d\n", getpid(), getppid());
								}else{
									waitpid(-1, &stat, 0);
								}
							}else{
								waitpid(-1, &stat, 0);
							}
						}else{
							waitpid(-1, &stat, 0);
						}
					}else{
						waitpid(-1, &stat, 0);
					}
				}else{
					waitpid(-1, &stat, 0);
				}
			}else{
				waitpid(-1, &stat, 0);
			}
		}else{
			waitpid(-1, &stat, 0);
		}
	}else{
		waitpid(-1, &stat, 0);
	}
}
