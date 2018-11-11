#ifndef BUFFER_H_INCLUDED
#define BUFFER_H_INCLUDED

#define TAM_STRING 255
#define TAM_BUFFER 10

/*Definicion del tipo String*/
typedef char String[TAM_STRING];

// typedef char* String;

/*Definicion del tipo buffer_t*/
typedef struct {
    char buffer[TAM_BUFFER][TAM_STRING];
    int inicio;
    int fin;
    int largo;
} buffer_t;

/*Funciones del buffer*/
void buffer_init(buffer_t *b);

int buffer_push(buffer_t *b, String dato);

int buffer_pop(buffer_t *b, String *dato);


#endif // BUFFER_H_INCLUDED
