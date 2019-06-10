#include <signal.h>
#include <stdio.h>

int i = 0, t = 0;

void maneja(int s){
	if(s == SIGTSTP){
		printf("Recibo SIGTSTP\n");
		t++;
	}
	if(s == SIGINT){
		printf("Recibo SIGINT\n");
		i++;
	}
}

int main(){
	printf("Esperando señales. PID = %i\n", getpid());
	//asigna manejador
	signal(SIGTSTP, maneja);
	signal(SIGINT, maneja);

	//Necesitamos un set vacio para el sigsuspend
	sigset_t set;
	sigemptyset(&set);

	while(i+t<10) sigsuspend(&set);

	printf("Recibidas %i SIGTSTP y %i SIGINT\n", t, i);
}
