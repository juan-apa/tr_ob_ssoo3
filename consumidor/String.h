#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED

#define TAM_STRING 255

/*Definicion del tipo String*/
typedef char string_a_t[TAM_STRING];
typedef char* string_p_t;

string_p_t string_apuntero(string_a_t str);

string_a_t* string_aarray(string_p_t str);

int string_largo_p(string_p_t str);

int string_largo_a(string_a_t str);

#endif // STRING_H_INCLUDED