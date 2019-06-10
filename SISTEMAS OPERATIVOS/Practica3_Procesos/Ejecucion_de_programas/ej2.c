#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[]){


  pid_t pid = fork();

  switch (pid) {
    case -1:
      perror("fork");
      exit(-1);
    break;
    case 0:;
	pid_t mi_sid = setsid(); //Creamos una nueva sesión
	printf("[Hijo] Proceso %i (Padre: %i)\n",getpid(),getppid());
	int fd = open("/tmp/daemon.out",O_CREAT | O_RDWR);
	int fderr = open("/tmp/daemon.err", O_CREAT | O_WRONLY);
	int null = open("/dev/null", O_CREAT | O_WRONLY);
	int fd2 = dup2(fd,1);
	int fd3 = dup2(fderr, 2);
	int fd4 = dup2(null, 0);
	execvp(argv[1], argv+1);
      break;
    default:
	wait(NULL);
        printf("[Padre] Proceso %i (Padre: %i)\n",getpid(),getppid());
        break;
  }

  return 0;
}
