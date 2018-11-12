#include <stdio.h>
#include <stdlib.h>
#include "String.h"
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <signal.h>
#include <pthread.h>    /* required for pthreads */
#include <fcntl.h>
#include "Buffer.h"
#include "DatosShMem.h"

int main()
{
    FILE * f;
    key_t claveMemoria;
    int idMemoria;
    datos_sh_mem_t* shMemData;
    sem_t * semaforo;

    /*Inicializo la memoria compartida*/
    f = fopen("/tmp/memCompartidatrOb", "w+");
    claveMemoria = ftok("/tmp/memCompartidatrOb", 33);
    idMemoria = shmget(claveMemoria, sizeof(datos_sh_mem_t), 0644 | IPC_CREAT);
    shMemData = (datos_sh_mem_t*) shmat(idMemoria, NULL, 0);

    /*Inicializo el semaforo*/
    semaforo = sem_open("sem_tr_ob1", O_CREAT, 0644, 1);

    int cont = 0;
    /*Inicio seccion critica*/
    sem_wait(semaforo);
    int finalizado = datos_sh_mem_finalizado(shMemData);
    sem_post(semaforo);
    /*fin seccion critica*/
    int salir = 0;
    while(salir == 0){
        string_p_t popeado = (char *) malloc(sizeof(char) * TAM_STRING);
        int pop;

        /*Inicio seccion critica*/
        sem_wait(semaforo);
        pop = buffer_pop(&(shMemData->bufferCirc), popeado);
        sem_post(semaforo);
        /*fin seccion critica*/

        if(pop == -1){
            printf("No se pudo popear nada.\n");
        }
        else{
            printf("Se popeo: %s\n", popeado);
        }
        sleep(1);
        cont++;
        /*Inicio seccion critica*/
        sem_wait(semaforo);
        int finalizado = datos_sh_mem_finalizado(shMemData);
        sem_post(semaforo);
        /*fin seccion critica*/
        printf("finalizado: %d\n", finalizado);
        
        if(pop == -1 && finalizado == 1){
            salir = 1;
        }
    }

    return 0;
}
