#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <fcntl.h>
#include "Buffer.h"

#define MAX_COMANDOS 10

//typedef struct char String[255];

void getNombreArchivo(String *ret){
    printf("Ingrese el nombre del archivo del cual leer los comandos: ");
    int i = 0;
    
    char c;
    scanf("%c", c);
    while(c != '\n'){
        *ret[i] = c;
        i++;
    }

    *ret[i] = '\0';
    printf("Leido de pantalla en getNombreArchivo: %s", ret);
}

FILE* archivo_init(String nombreArchivo){
    FILE* ret = NULL;
    ret = fopen(nombreArchivo, "r");
    if(ret == NULL){
        printf("No se ha encontrado el archivo en el fs.\n");
    }
    return ret;
}

void archivo_cerrar(FILE* f){
    fclose(f);
}

void invocarConsumidor(String comando){
    
}

int main()
{
    String nombreArchivo;
    FILE* archivo;
    String linea;
    size_t largoArch = 0;
    __ssize_t largoLinea;
    buffer_t buffer;
    sem_t * sem_a;

    /*Inicializo el semaforo*/
    //sem_a = sem_open("sem_tr_ob", O_CREAT, 0644, 0);

    /*Inicializo el buffer*/
    // buffer = malloc(sizeof(buffer_t));
    buffer_init(&buffer);

    /*Obtengo el nombre del archivo del cual leer los datos*/
    getNombreArchivo(&nombreArchivo);

    /*Inicializo el archivo*/
    archivo = archivo_init(nombreArchivo);
    if(archivo == NULL){
        printf("Archivo = null\n");
    }

    /*Leo las lineas una por una y las mando a los consumidores*/
    //linea = malloc(sizeof(char)*TAM_STRING);
    while ((largoLinea = getline(&linea, &largoArch, archivo)) != -1) {
        // printf("Largo: %zu; contenido: %s", largoLinea, linea);
        /*Como linea tiene cargado el string con el \n, tengo que sacarselo al final de los string
        y ademas tengo que chequear que no sea \n solo (ultima linea en archivos UNIX)*/
        /*Saco el \n del string*/
        if(linea[strlen(linea)-1] == '\n' ){
            linea[strlen(linea) - 1] = '\0';
        }
        /*Si la linea no esta vacia, la inserto al buffer*/
        if(linea[0] != '\0'){
            printf("insertando..\n");
            buffer_push(&buffer, *linea);
        }
        
        // linea = malloc(sizeof(char)*TAM_STRING);
    }

    /*Cierro el archivo*/
    archivo_cerrar(archivo);

    // printf("inicio script.\n");
    // buffer_t *buffer = malloc(sizeof(buffer_t));
    // buffer_init(buffer, 10);
    // buffer_push(buffer, "dato_1");
    // buffer_push(buffer, "dato_2");
    // buffer_push(buffer, "dato_3");
    // buffer_push(buffer, "dato_4");

    // for(int i = 0; i < 4; i++){
    //     String datoLeidoBuffer = malloc(sizeof(char) * TAM_STRING);
    //     buffer_pop(buffer, datoLeidoBuffer);
    //     printf("Obtenido: %s\n", datoLeidoBuffer);
    //     free(datoLeidoBuffer);
    // }

    String* datoLeidoBuffer = malloc(sizeof(char) * TAM_STRING);
    while(buffer_pop(&buffer, datoLeidoBuffer) == 0){
        printf("Obtenido: %s\n", datoLeidoBuffer);
        free(datoLeidoBuffer);
        // datoLeidoBuffer = malloc(sizeof(char) * TAM_STRING);
    }
    free(datoLeidoBuffer);
    
    return 0;
}
