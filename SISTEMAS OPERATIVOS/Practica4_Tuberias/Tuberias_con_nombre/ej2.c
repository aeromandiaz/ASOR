#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>


int main(int argc, char **argv) {
	if (argc < 2) return -1;

  int fd = open("./tuberia", O_WRONLY);
	if (fd == -1) {
		perror("Unable to open the pipe");
		close(fd);
		return -1;
	}

	if (write(fd, argv[1], strlen(argv[1])) == -1) {
		perror("Unable to write on the pipe");
		close(fd);
		return -1;
	}

	close(fd);
	return 0;
}
