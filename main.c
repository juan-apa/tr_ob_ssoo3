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

#define MAX_COMANDOS 10


void getNombreArchivo(string_p_t ret){
    printf("Ingrese el nombre del archivo del cual leer los comandos: ");
    int i = 0;

    char c;
    scanf("%c", &c);
    while(c != '\n'){
        ret[i] = c;
        scanf("%c", &c);
        i++;
    }

    ret[i] = '\0';
}

FILE* archivo_init(string_p_t nombreArchivo){
    FILE* ret = NULL;
    ret = fopen(nombreArchivo, "r");
    return ret;
}

void archivo_cerrar(FILE* f){
    fclose(f);
}

int main()
{
    string_p_t nombreArchivo = malloc(sizeof(char) * TAM_STRING);
    FILE* archivo;
    string_p_t linea;
    size_t largoArch = 0;
    __ssize_t largoLinea;
    datos_sh_mem_t* shMemData;
    sem_t * semaforo;

    FILE * f;
    key_t claveMemoria;
    int idMemoria;

    /*Inicializo la memoria compartida*/
    f = fopen("/tmp/memCompartidatrOb", "w+");
    claveMemoria = ftok("/tmp/memCompartidatrOb", 33);
    idMemoria = shmget(claveMemoria, sizeof(datos_sh_mem_t), 0644 | IPC_CREAT);
    shMemData = (datos_sh_mem_t*) shmat(idMemoria, NULL, 0);

    /*Inicializo el semaforo*/
    semaforo = sem_open("sem_tr_ob1", O_CREAT, 0644, 1);

    /*Inicializo el buffer*/
    datos_sh_mem_init(shMemData, 0);

    /*Obtengo el nombre del archivo del cual leer los datos*/
    getNombreArchivo(nombreArchivo);

    /*Inicializo el archivo*/
    archivo = archivo_init(nombreArchivo);

    /*Me fijo si existe el archivo*/
    if(archivo == NULL){
        printf("No se ha encontrado el archivo en el fs.\n");
    }else{
        /*Leo las lineas una por una y las mando a los consumidores*/
        while ((largoLinea = getline(&linea, &largoArch, archivo)) != -1) {
            /*Como linea tiene cargado el string con el \n, tengo que sacarselo al final de los string
            y ademas tengo que chequear que no sea \n solo (ultima linea en archivos UNIX)*/
            /*Saco el \n del string*/
            if(linea[string_largo_p(linea)-1] == '\n' ){
                linea[string_largo_p(linea) - 1] = '\0';
            }
            /*Si la linea no esta vacia, la inserto al buffer*/
            if(linea[0] != '\0'){
                printf("insertando..\n");

                /*Inicio seccion critica*/
                sem_wait(semaforo);
                buffer_push(&(shMemData -> bufferCirc), linea);
                sem_post(semaforo);
                /*Fin seccion critica*/
            }
        }

        /*Cierro el archivo*/
        archivo_cerrar(archivo);
        datos_sh_mem_finalizar(shMemData);
        printf("Finalizado: %d\n", shMemData ->finArchivo);
        int cont = 0;
        while(cont < 10){
            sleep(1);
            cont++;
        }
    }

    /*Me desconecto de la memoria compartida*/
    shmctl (idMemoria, IPC_RMID, (struct shmid_ds *)NULL);
    unlink ("/tmp/memCompartidatrOb");

    return 0;
}
