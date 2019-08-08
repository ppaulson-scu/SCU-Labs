#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

//A program that creates seventeen subprocesses.
//Design: The original program creates three children. Two of those children create three children; one creates two children. The two children with the same parent create three children each.
//Inut: n/a
//Output: Each process prints its PID and its parent's PID. The original process identifies itself.
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
			doubleFork();
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
			}
		}
	}
}

int doubleFork(){

	int status;
	pid_t mid_left, mid_right;

	mid_left = fork();
	
	if(mid_left == 0){
		printf("My PID is %d, my parent's PID is %d\n", getpid(), getppid());
		tripleFork();
		return 0;
	}else{
		waitpid(-1, &status, 0);
		mid_right = fork();
		if(mid_right == 0){
			printf("My PID is %d, my parent's PID is %d\n", getpid(), getppid());
			tripleFork();
			return 0;
		}else{
			waitpid(-1, &status, 0);
		}
	}
}
