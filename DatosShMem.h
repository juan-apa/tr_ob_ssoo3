#ifndef DATOSSHMEM_H_INCLUDED
#define DATOSSHMEM_H_INCLUDED

#include "Buffer.h"

typedef struct{
    buffer_t bufferCirc;
    int finArchivo;
}datos_sh_mem_t;

void datos_sh_mem_init(datos_sh_mem_t* datos, int finArchivo);

int datos_sh_mem_finalizado(datos_sh_mem_t* datos);

int datos_sh_mem_finalizar(datos_sh_mem_t* datos);

#endif // DATOSSHMEM_H_INCLUDED