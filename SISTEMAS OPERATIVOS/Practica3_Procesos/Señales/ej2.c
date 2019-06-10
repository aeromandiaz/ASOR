#include <stdio.h>

int main(){
	printf("PID: %i", getpid());
	fflush(stdout);
	sleep(600);
}
