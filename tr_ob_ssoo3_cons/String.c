#include "String.h"

int string_largo_p(string_p_t str){
    int ret = 0;
    while(ret < TAM_STRING && str[ret] != '\0'){
        ret++;
    }
    return ret; //Largo sin el \0
}

int string_largo_a(string_a_t str){
    int ret = 0;
    while(ret < TAM_STRING && str[ret] != '\0'){
        ret++;
    }
    return ret; //Largo sin el \0
}

string_p_t string_apuntero(string_a_t str){
    string_p_t ret;
    int largoArr = string_largo_a(str);
    ret = malloc(sizeof(char) * (largoArr + 1));

    for(int i = 0; i < largoArr; i++){
        ret[i] = str[i];
    }
    ret[largoArr] = '\0';
    return ret;
}

string_a_t* string_aarray(string_p_t str){
    string_a_t ret;
    int largoArr = string_largo_p(str);

    for(int i = 0; i < largoArr; i++){
        ret[i] = str[i];
    }
    ret[largoArr] = '\0';
    return &ret;
}



