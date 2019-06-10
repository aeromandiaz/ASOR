#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

void detiene(int s){
	printf("Detenido el borrado\n");
	exit(0);
}

void borra(int s){
	printf("Borrando\n");
	remove("./ej5");
	exit(0);
}

int main(char argc, char *argv[]){
	int a = atoi(argv[1]);
	printf("Esperando señales, borrado en %i segundos.\n PID = %i\n", a, getpid());
	//asigna manejador
	signal(SIGALRM, borra);
	signal(SIGUSR1, detiene);

	alarm(a);
	//Necesitamos un set vacio para el sigsuspend
	sigset_t set;
	sigemptyset(&set);

	while(2) sigsuspend(&set);
}
