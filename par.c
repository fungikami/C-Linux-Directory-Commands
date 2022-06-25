/**
 * Par.c
 * Par de palabras
 * Autor: Ka Fung (1810492)
 * Fecha: 16/06/2020 
 */

#include <stdio.h>
#include <stdlib.h>
#include "par.h"

/**
 * Crea un nuevo par de palabras.
 * Parametros:
 *     - x: palabra1
 *     - y: palabra2
 * Retorno:
 *    - par: par de palabras
 */
Par* crear_par(char* x, char* y) {
    Par* par = (Par*)malloc(sizeof(Par));
    if(!par) {
        return NULL;
    }

    par->x = x;
    par->y = y;
    return par;
}

/**
 * Imprime un par de palabras.
 * Parametros:
 *    - par: par de palabras
 */ 
void print_par(Par* par) {
    printf("(%s, %s)", par->x, par->y);
}

/**
 * Libera la memoria asignada de un par de palabras.
 * Parametros:
 *   - par: par de palabras
 */
void liberar_par(Par* par) {
    free(par->x);
    free(par->y);
    free(par);
}