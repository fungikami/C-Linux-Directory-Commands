/**
 * main.c
 * 
 * Autor: Ka Fung (1810492)
 * Fecha: 08/07/2020 
 */

#include <stdio.h>
#include <stdlib.h>
#include "finds.h"
#include "repla.h"
#include "wc.h"
#include "codif.h"
#include "roll.h"

int main(int argc, char **argv) {
    printf("Directorio raiz: %s\n", argv[1]);
    printf("String a buscar: %s\n", argv[2]);
    printf("String a buscar contenido: %s\n", argv[3]);
    printf("File con palabras a reemplazar: %s\n", argv[4]);
    printf("Roll con n: %s\n", argv[5]);
    
    printf("\nProbando find\n");
    find(argv[1], argv[2]);

    printf("\nProbando ifind\n");
    ifind(argv[1], argv[2]);

    printf("\nProbando cfind con la palabra %s\n", argv[3]);
    cfind(argv[1], argv[2], argv[3]);

    /* printf("\nProbando repla\n");
    args->lista = extraer_palabras(argv[4]);
    traverseDir(directorioRaiz, repla, args, 0); */

    printf("\nProbando wc\n");
    wc(argv[1]);

    printf("\nProbando codif\n");
    codif(argv[1]);

    printf("\nProbando roll\n");
    /* roll(argv[1], argv[5]); */
    return 0;
}

