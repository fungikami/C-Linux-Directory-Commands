#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "utilidades.h"

int codif_aux(char *path, struct Args *args);
int roll_aux(char *path, struct Args *args);

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
    int izq, der;
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

/**
 * Función que llama la función auxiliar roll para rotar n  
 * caracteres del contenido de los archivos de un directorio raíz.
 * 
 * Parámetros:
 *      directorioRaiz: ruta del directorio raíz 
 *      n: número de caracteres a rotar
 */
void roll(char *directorioRaiz, int n) {
    struct Args* args = (struct Args*)malloc(sizeof(struct Args));
    if (!args) {
        fprintf(stderr, "Error al reservar memoria\n");
    }
    args->n = n;
    if (traverseDir(directorioRaiz, roll_aux, args, 0) == -1) {
        fprintf(stderr, "Error al ejecutar roll.\n");
    }
    free(args);
}

/**
 * Función que rota n caracteres el contenido de un archivo
 * Si n es positivo, rota hacia la derecha
 * Si n es negativo, rota hacia la izquierda
 * 
 */
int roll_aux(char *path, struct Args *args) {   
    int n = args->n;
    int fd = open(path, O_RDWR);
    int len1, len2, i;
    char *buf = malloc(n);
    char *buf_roll = malloc(n);
    read(fd, buf_roll, n);

    while ((len1 = read(fd, buf, n)) > 0) {
        printf("%s %d\n", buf, len1);
        len2 = 0;
        do {

            lseek(fd, -len1-n, SEEK_CUR);
            i = write(fd, buf + len2, len1 - len2);
            printf("%d\n", i);
            len2 += i;
            lseek(fd, len2+(len1-len2), SEEK_CUR);

        } while (len2 < len1);
    }

    close(fd);
    return 0;
}