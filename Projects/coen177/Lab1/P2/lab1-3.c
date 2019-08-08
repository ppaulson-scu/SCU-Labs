#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

//A program that creates twelve subprocesses.
//Design: initial program creates three child processes, which each create three children of their own.
//Input: n/a
//Output: Each process prints its PID, plus its parent's PID. The original process identifies itself.
int main(){

	int status;
	int counter = 0;
	pid_t top_left, top_center, top_right;
	

	top_left = fork();

	if(top_left == 0){
		printf("My PID is %d, my parent's PID is %d\n", getpid(), getppid());
		tripleFork();
		return 0;
	}else{
		waitpid(-1, &status, 0);
		top_center = fork();
		
		if(top_center == 0){
			printf("My PID is %d, my parent's PID is %d\n", getpid(), getppid());
			tripleFork();
			return 0;
		}else{
			waitpid(-1, &status, 0);
			top_right = fork();
			
			if(top_right == 0){
				printf("My PID is %d, my parent's PID is %d\n", getpid(), getppid());
				tripleFork();
				return 0;
			}else{
				waitpid(-1, &status, 0);
				printf("I'm the original! My PID is %d, my parent's PID is %d\n", getpid(), getppid());
			}
		}
	}
}

int tripleFork(){

	int status;
	pid_t left, center, right;

	left = fork();

	if(left == 0){
		printf("My PID is %d, my parent's PID is %d\n", getpid(), getppid());
		return 0;
	}else{
		waitpid(-1, &status, 0);
		center = fork();
		
		if(center == 0){
			printf("My PID is %d, my parent's PID is %d\n", getpid(), getppid());
			return 0;
		}else{
			waitpid(-1, &status, 0);
			right = fork();
			
			if(right == 0){
				printf("My PID is %d, my parent's PID is %d\n", getpid(), getppid());
				return 0;
			}else{
				waitpid(-1, &status, 0);
				//printf("My PID is %d, my parent's PID is %d\n", getpid(), getppid());
			}
		}
	}
}
