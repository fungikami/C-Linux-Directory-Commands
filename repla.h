/**
 * repla.h
 * 
 * Autor: Ka Fung (1810492)
 * Fecha: 16/06/2020 
 */

#ifndef __REPLA_H__
    #define __REPLA_H__

    #include <stdio.h>
    #include "lista.h"

    char *random_string(int length);
    struct Nodo* extraer_palabras(char* archivo);
    int remplazar_palabras(char* archivo, struct Nodo* cabeza);
#endif