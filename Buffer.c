#include "Buffer.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>


void buffer_init(buffer_t *b){
    for(int i = 0; i < TAM_BUFFER; i++){
        for(int j = 0; j < TAM_STRING; j++){
            b -> buffer[i][j] = '0';
            if(j == 0){
                b -> buffer[i][j] = '\0';
            }
        }
    }
    b -> largo = TAM_BUFFER;
    b -> inicio = 0;
    b -> fin = 0;
}

int buffer_push(buffer_t *b, string_p_t dato){
    int ret = 0;

    int siguiente;

    siguiente = b -> inicio + 1;
    if (siguiente >= b -> largo)
        siguiente = 0;

    if (siguiente == b -> fin)
        ret = -1;

    if(ret == 0){
        int i = 0;
        while(i < TAM_STRING && dato[i] != '\0'){
            b -> buffer[b -> inicio][i] = dato[i];
            i++;
        }
        b -> buffer[b -> inicio][i] = '\0';
        // for(int i = 0; i < TAM_STRING; i++){
        //     b -> buffer[b -> inicio][i] = dato[i];
        // }
        //b -> buffer[b -> inicio] = dato;
        // printf("bpush: %s\n", b -> buffer[b -> inicio]);
        b -> inicio = siguiente;
    }

    return ret;
}

int buffer_pop(buffer_t *b, string_p_t dato){
    int ret = 0;

    int siguiente;

    if (b -> inicio == b -> fin)
        ret = -1;

    if(ret == 0){
        siguiente = b -> fin + 1;

        if(siguiente >= b -> largo){
            siguiente = 0;
        }
        int largo = string_largo_a(b -> buffer[b -> fin]) + 1;
        int i = 0;
        while(i < TAM_STRING && b -> buffer[b -> fin][i] != '\0'){
            dato[i] = b -> buffer[b -> fin][i];
            i++;
        }
        dato[i] = '\0';
//        for(int i = 0; i < TAM_STRING; i++){
//            *dato[i] = b -> buffer[b -> fin][i];
//        }
        printf("\nStringLeido: %s\n", dato);
        // String aux = b -> buffer[b -> fin];
        // char c = aux[0];
        // int i = 0;
        // while(c != '\0' /* && c != '\n'*/){
        //     dato[i] = c;
        //     i++;
        //     c = aux[i];
        // }
        // aux[i+1] = '\0';
        // printf("bpop: %s\n", dato);
        b -> fin = siguiente;
    }
    return ret;
}
