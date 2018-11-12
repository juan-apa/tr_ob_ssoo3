#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <signal.h>
#include <pthread.h>    /* required for pthreads */
#include "Buffer.h"
#include "DatosShMem.h"
#include "String.h"

int main()
{
    FILE * f;
    key_t claveMemoria;
    int idMemoria;
    datos_sh_mem_t* shMemData;

    f = fopen("/tmp/memCompartidatrOb", "w+");
    claveMemoria = ftok("/tmp/memCompartidatrOb", 33);
    idMemoria = shmget(claveMemoria, sizeof(datos_sh_mem_t), 0644 | IPC_CREAT);
    shMemData = (datos_sh_mem_t*) shmat(idMemoria, NULL, 0);

    int cont = 0;
    while(datos_sh_mem_finalizado(shMemData) == 0 && cont < 10){
        string_p_t popeado = (char *) malloc(sizeof(char) * TAM_STRING);
        int pop;
        pop = buffer_pop(&(shMemData->bufferCirc), popeado);
        if(pop == -1){
            printf("No se pudo popear nada.\n");
        }
        else{
            printf("Se popeo: %s\n", popeado);
        }
        sleep(1);
        cont++;
    }

    return 0;
}
