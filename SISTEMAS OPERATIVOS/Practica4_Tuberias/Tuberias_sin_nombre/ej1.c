#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	int fd[2];
	pipe(fd);

	if(fork() == 0){//HIJO
		close(fd[STDOUT_FILENO]);
		dup2(fd[STDIN_FILENO], STDIN_FILENO);
		close(fd[STDIN_FILENO]);
		execlp(argv[3], argv[3], argv[4], NULL);
	}else{//PAE
		close(fd[STDIN_FILENO]);
		dup2(fd[STDOUT_FILENO], STDOUT_FILENO);
		close(fd[STDOUT_FILENO]);
		execlp(argv[1], argv[1], argv[2], NULL);
	}
}
