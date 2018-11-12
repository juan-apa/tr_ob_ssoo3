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
#include <time.h>
#include <string.h>


int escribirSalidaComando(string_p_t comando, string_p_t str){
    int ret = 0;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    string_p_t horaAhora;
    sprintf(horaAhora, "%d-%d-%d_%d:%d_", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min);
    strcat(horaAhora, comando);
    strcat(horaAhora, ".log");
    
    FILE *f = fopen(horaAhora, "w");
    if (f == NULL)
    {
        printf("Error al abrir el archivo de salida de comandos: %s\n", horaAhora);
        ret = -1;
    }
    else{
        fprintf(f, str);
        fclose(f);
    }
    free(horaAhora);
    return ret;
}

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
            /*Ejecuto el comando*/
            FILE *fp = popen(command,"r");
            string_p_t salidaComando;
            fscanf(fp, salidaComando);
            escribirSalidaComando(popeado, salidaComando);
            pclose(fp);
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
