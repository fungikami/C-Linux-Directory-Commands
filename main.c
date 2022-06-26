/**
 * main.c
 * 
 * Autor: Ka Fung (1810492)
 * Fecha: 08/07/2020 
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utilidades.h"

int main(int argc, char **argv) {
    char* directorioRaiz;
    char* string1;
    struct Args* args;
    int lineas = 0;
    int chars = 0;
    char *path;

    clock_t t;
    double time_taken;

    /* if (argc != 2) {
        printf("Uso: ./myutil <directorioRaiz>\n");
        return 1;
    } */
    directorioRaiz = argv[1];
    string1 = argv[2];

    printf("Directorio raiz: %s\n", directorioRaiz);
    printf("String a buscar: %s\n", string1);

    /* Crea una estructura para guardar los argumentos */
    args = (struct Args*)malloc(sizeof(struct Args));
    if (!args) {
        printf("Error al reservar memoria\n");
        return 1;
    }
    args->path = directorioRaiz;
    args->cadena1 = string1;
    args->cadena2 = argv[3];
    args->n = 0;

    /*  while () {
        printf("myutil> ");
        scanf("%[^\n]", args->dirRaiz);
    } */

    
    /* */
    printf("\nProbando find\n");
    traverseDir(directorioRaiz, find, args, 0);

    printf("\nProbando ifind\n");
    traverseDir(directorioRaiz, ifind, args, 0);

    printf("\nProbando cfind con la palabra %s\n", argv[3]);
    traverseDir(directorioRaiz, cfind, args, 0);

    printf("\nProbando repla\n");
    /* Encuentra archivo de las palabras y las extrae */
    /*  args->to_print = 0;
    traverseDir(directorioRaiz, find, args, 0);
    printf("%s\n", args->path); 
    args->palabras = extraer_palabras(args->path);
    repla(args); */

    printf("\nProbando wc\n");
    wc(directorioRaiz, &lineas, &chars);

    printf("\nProbando codif\n");
    traverseDir(directorioRaiz, codif, args, 0);

    printf("\nProbando roll\n");
    /* traverseDir(directorioRaiz, roll, args, 0); */
    /* roll("roll.txt", 3); */
    return 0;
}

