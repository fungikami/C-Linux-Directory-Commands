/**
 * main.c
 * 
 * Autor: Ka Fung (1810492)
 * Fecha: 08/07/2020 
 */

#include <stdio.h>
#include "utilidades.h"

int main(int argc, char **argv) {
    char* rootDir;
    int lines = 0;
    int chars = 0;

    if (argc != 2) {
        printf("Uso: ./myutil <directorioRaiz>\n");
        return 1;
    }

    rootDir = argv[1];
    printf("Buscando en %s\n", rootDir);

    traverseDir(rootDir, "txt");

    codif2(rootDir);
    wc(rootDir, &lines, &chars);
    printf("%d lineas, %d caracteres\n", lines, chars);
    return 0;
}
