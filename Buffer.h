#ifndef BUFFER_H_INCLUDED
#define BUFFER_H_INCLUDED

#define TAM_BUFFER 10

/*Definicion del tipo buffer_t*/
typedef struct {
    char buffer[TAM_BUFFER][TAM_STRING];
    int inicio;
    int fin;
    int largo;
} buffer_t;

/*Funciones del buffer*/
void buffer_init(buffer_t *b);

int buffer_push(buffer_t *b, string_a_t dato);

int buffer_pop(buffer_t *b, string_a_t *dato);


#endif // BUFFER_H_INCLUDED
