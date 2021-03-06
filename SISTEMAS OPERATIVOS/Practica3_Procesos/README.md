# Práctica 2.3.Procesos

## Tabla de contenido
- [Políticas de planificación](#políticas-de-planificación).
- [Grupos de procesos y sesiones Recursos de un proceso](#grupos-de-procesos-y-sesiones-recursos-de-un-proceso).
- [Ejecución de programas](#ejecución-de-programas).
- [Señales](#señales).



## Políticas de planificación

### Ejercicio 1
#### Comando chrt
Sirve para consultar y cambiar la planificación de un proceso así como para consultar su prioridad.

```bash
$man chrt
```

<pre>
<code>$chrt -v -p <pid>
</code></pre>

Para cambiar la planificación

<pre>
<code>$chrt -o -p <pid> #SCHED_OTHER
$chrt -f -p <pid> #SCHED_FIFO
$chrt -r -p <pid> #SCHED_RR
</code></pre>

#### Comando nice
Sirve para modificar la prioridad (modificar el valor nice de un proceso).

<pre>
<code>$man nice
$man renice
</code></pre>

<pre>
<code>$sudo nice -n-10 /bin/sh
$sudo chrt -f -p 12 22152
</code></pre>


### Ejercicio 2

1.- Para obtener la politica de plaificacion del proceso actual, hacemos la siguiente llamada

```c
sched_getscheduler(getpid());
```

2.- Para obtener una propiedad de la politica de planificacion:

```c
//Estructura para almacenar los distintos parametros
struct sched_param p;
sched_getparam(getpid(),&p);
//nos interesa el parametro sched_priority
printf("pid %i's current scheduling priority: %i\n", getpid(), p.sched_priority);
```
3.- Para el maximo y el minimo hacemos las llamadas:

```c
sched_get_priority_max(int scheduler);
sched_get_priority_min(int scheduler);
```

Solución completa: [Mostrar politica](Politicas_de_planificacion/ej2.c)

### Ejercicio 3

```bash
$ echo $$
2278
$ chrt -f -p 12 2278
$ ./ej2
pid 7840's current scheduling policy: SCHED_FIFO
pid 7840's current scheduling priority: 12
MAX: 99 - MIN: 1
```
Si, se heredan

## Grupos de procesos y sesiones Recursos de un proceso

### Ejercicio 1

<pre>
<code>$man ps
</code></pre>

**ps -u $USER -f** Mostrar todos los procesos del usuario actual en formato extendido.
**ps -eo pid,gid,sid,s,command** Mostrar los procesos del sistema, incluyendo PID, del grupo, la sesión, el estado y la línea de comandos.

a) El PID(y el SID) de la shell es el SID del nuevo proceso. Comparten el GID (1000)
b) 1000

### Ejercicio 2
Para obtener el espacio de trabajo utilizamos la función:

```c
#include <unistd.h>
char *getcwd(char *buf, size_t size);
```

Solución completa: [ej2](Grupo_de_procesos_y_sesiones_Recursos_de_un_proceso/ej2.c)

### Ejercicio 3

Solución completa: [Plantilla de demonio](Grupo_de_procesos_y_sesiones_Recursos_de_un_proceso/ej3.c)

¿Qué pasa si el padre termina antes que el hijo?

El hijo se queda huérfano y el ppid lo recoge la shell o init.

¿Qué pasa si el hijo termina antes que el padre?

El proceso se queda como *defunt* como podemos ver en la siguiente imagen:

![ps](ps.png)

## Ejecución de programas

### Ejercicio 1

[ej1_exec](Ejecucion_de_programas/ej1_exec.c)

[ej1_system](Ejecucion_de_programas/ej1_system.c)

La cadena "El comando terminó de ejecutarse" sólo se ejecuta cuando se usa system. Esto se debe al ejecutar el comando exec sustituye la imagen del programa a la imagen del programa que hemos pasado por argumentos.

Diferencia entre ps -el y "ps -el"

Cuando se pasa por parámetros de un programa ps -ef equivaldría a dos argumentos y si se quiere ejecutar como systme sería necesario unirlos. Mientras que "ps -el" equivale a un único string lo que nos permitiría ejecutar directamente el comando system sin necesidad de unirlos.

### Ejercicio 2

Solución completa: [Exec demonio](Ejecucion_de_programas/ej2.c)

## Señales

### Ejercicio 1

<pre>
<code>$man kill
$man pkill
</code></pre>

Envía una señal a un proceso. Uso:

<pre>
<code>$kill <pid> #Envía la señal de KILL a un proceso.
$kill -9 <pid>
$kill -s <pid> #Manda una señal.
$kill -l #Lista todas las señales.
</code></pre>

Todas las señales del sistema:

![Lista de Señales](listaseñales.png)

### Ejercicio 2

Para facilitar el proceso utilizamos un programilla para el sleep

Solución completa: [Programa sleep](Señales/ej2.c)

*Terminal 1*

<pre>
<code>$ ./sleep
</code></pre>

*Terminal 2*

<pre>
<code>$ kill -s [Signal_name] [Sleep_PID]  
</code></pre>

**SIGHUP:**

Comportamiento: Se ha terminado el proceso (Se ha desconectado de la SHELL).

Salida:Colgar (hangup)

**SIGINT:**

Comportamiento: Se ha interrumpido el sleep.

Salida:N/A

**SIGQUIT:**

Comportamiento: Se ha interrumpido el sleep.

Salida: Abandona (core generado)

**SIGILL:**

Comportamiento: Se ha interrumpido el sleep.

Salida:Instrucción ilegal (core generado)

**SIGTRAP:**

Comportamiento: Se ha interrumpido el sleep

Salida: «trap» para punto de parada/seguimiento (core generado)

**SIGKILL:**

Comportamiento: Se ha interrumpido el sleep

Salida: Terminado (killed)

**SIGBUS:**

Comportamiento: Se ha interrumpido el sleep

Salida: Error del bus (core generado)

**SIGSEGV:**

Comportamiento: Se ha interrumpido el sleep

Salida: Violación de segmento (core generado)

**SIGPIPE:**

Comportamiento: Se ha interrumpido el sleep

Salida: N/A (Ha salido una notificación en la pantalla).

**SIGTERM:**

Comportamiento:  Se ha interrumpido el sleep

Salida: Terminado

### Ejercicio 3

[Script variable de entorno](Señales/ej3.sh)

Solución completa: [Programa bloqueo](Señales/ej3.c)

### Ejercicio 4

Solución completa: [Contador de señales](Señales/ej4.c)

### Ejercicio 5

Como nos dicen que no podemos usar la función sleep, usamos la señal alarm con un manejador.

Solución completa: [Borrado automatico](Señales/ej5.c)
