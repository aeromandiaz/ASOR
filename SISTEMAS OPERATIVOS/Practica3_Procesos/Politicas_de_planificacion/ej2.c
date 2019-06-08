#include <unistd.h>
#include <stdio.h>
#include <sched.h>

int main(int argc, char *argv[]){
	int pid = getpid();
	//obtenemos la politica
	int politica = sched_getscheduler(pid);
	switch (politica){//imprimimos el resultado
		case SCHED_OTHER:
			printf("pid %i's current scheduling policy: SCHED_OTHER\n", pid);
		break;
		case SCHED_FIFO:
			printf("pid %i's current scheduling policy: SCHED_FIFO\n", pid);
		break;
		case SCHED_RR:
			printf("pid %i's current scheduling policy: SCHED_RR\n", pid);
		break;
		default:
			printf("SCHEDULER: ERROR\n");
		break;
	}

	//Estructura para almacenar los distintos parametros
	struct sched_param p;
  	sched_getparam(pid,&p);
	//nos interesa el parametro sched_priority
	printf("pid %i's current scheduling priority: %i\n", pid, p.sched_priority);

	int max = sched_get_priority_max(politica);
	int min = sched_get_priority_min(politica);
	printf("MAX: %i - MIN: %i\n", max, min);
}
