#include "DatosShMem.h"

void datos_sh_mem_init(datos_sh_mem_t* datos, int finArchivo){
    buffer_init(&(datos->bufferCirc));
    datos->finArchivo = 0;
}

int datos_sh_mem_finalizado(datos_sh_mem_t* datos){
    return datos -> finArchivo;
}

void datos_sh_mem_finalizar(datos_sh_mem_t* datos){
    datos -> finArchivo = 1;
}