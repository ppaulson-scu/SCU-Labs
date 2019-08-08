#include <stdio.h>
#include <stdlib.h>

//A simple hello world.
int main(){
	time_t t;
	srand(78123);
	while(1){ printf("%d\n", rand() % 101); }
}
