#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "utilidades.h"

int roll_aux(char *path, struct Args *args);

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