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
    int m = abs(n);
    int fd = open(path, O_RDWR);
    int filesize;
    char *buffer_n = (char*)malloc(m);
    char *buffer;

    /* Verifica que el archivo fue abierto */
    if (fd == -1) {
        fprintf(stderr, "Error al abrir el archivo %s\n", path);
        return -1;
    }

    filesize = lseek(fd, -1, SEEK_END);
    buffer = (char*)malloc(filesize - m);

    /* Si n es positivo, se rota hacia la derecha */
    if (n >= 0) {
        /* Lee los ultimos n caracteres */
        lseek(fd, -m-1, SEEK_END);
        read(fd, buffer_n, m);

        /* Rota n caracteres desde el principio */
        lseek(fd, 0, SEEK_SET);
        read(fd, buffer, filesize - m);

        /* Escribe los nuevos caracteres */
        lseek(fd, m, SEEK_SET);
        write(fd, buffer, filesize - m);
        lseek(fd, 0, SEEK_SET);
        write(fd, buffer_n, m);
    } else {
        /* Si n es negativo, se rota hacia la izquierda */
        
        /* Lee los primeros n caracteres */
        lseek(fd, 0, SEEK_SET);
        read(fd, buffer_n, m);

        /* Rota n caracteres desde el final */
        read(fd, buffer, filesize - m);

        /* Escribe los nuevos caracteres */
        lseek(fd, 0, SEEK_SET);
        write(fd, buffer, filesize - m);
        write(fd, buffer_n, m);
    }

    free(buffer_n);
    free(buffer);
    close(fd);

    return 0;
}