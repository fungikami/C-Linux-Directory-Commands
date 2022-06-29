#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "utilidades.h"

int codif_aux(char *path, struct Args *args);

/**
 * Función que llama la función auxiliar codif_aux
 * para invertir el contenido de los archivos de un directorio raíz.
 * 
 * Parámetros:
 *      directorioRaiz: ruta del directorio raíz 
 */
void codif(char *directorioRaiz) {
    struct Args* args = (struct Args*)malloc(sizeof(struct Args));
    if (!args) {
        fprintf(stderr, "Error al reservar memoria\n");
    }
    if (traverseDir(directorioRaiz, codif_aux, args, 0) == -1) {
        fprintf(stderr, "Error al ejecutar codif.\n");
    }
    free(args);
}

/**
 * Función que invierte el contenido de un archivo
 * 
 * Parámetros:
 *      path: ruta del archivo a invertir el contenido
 * Retorno:
 *      0 si todo fue correcto, -1 si hubo un error
 */
int codif_aux(char *path, struct Args *args) {
    char izq, der;
    int fd = open(path, O_RDWR);
    long m, n, filesize;

    /* Verifica que el archivo fue abierto */
    if (fd == -1) return -1;

    filesize = lseek(fd, -1, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    m = 0;
    n = filesize / 2;
    while (n) {
        /* Lee el caracter más izquierdo a intercambiar */
        if (lseek(fd, m++, SEEK_SET) == -1) return -1;
        if (read(fd, &izq, 1) == -1) return -1;

        /* Lee el caracter más derecho a intercambiar */
        if (lseek(fd, -m, SEEK_END) == -1) return -1;
        if (read(fd, &der, 1) == -1) return -1;

        /* Escribe el caracter más derecho en la izquierda*/
        if (lseek(fd, -m, SEEK_END) == -1) return -1;
        if (write(fd, &izq, 1) == -1) return -1;

        /* Escribe el caracter más izquierdo en la derecha*/
        if (lseek(fd, m-1, SEEK_SET) == -1) return -1;
        if (write(fd, &der, 1) == -1) return -1;

        n--;
    }

    /* Cierra el archivo */
    close(fd);
    return 0;
}