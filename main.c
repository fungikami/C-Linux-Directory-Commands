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
    args->cadena2 = NULL;
    args->file = NULL;
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

    /* codif(dirRaiz);

    wc(dirRaiz, &lineas, &chars);

    lineas = 0;
    chars = 0;
    t = clock();
    count_lines_chars(dirRaiz, &lineas, &chars);
    t = clock() - t;
    time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("%d lineas, %d caracteres\n", lineas, chars);
    printf("Tiempo de ejecucion: %f segundos\n", time_taken); */

    return 0;
}

