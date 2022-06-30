/**
 * roll.c
 * Implementación del comando roll, en la cual rota n caracteres el contenido
 * de cada archivo regular desde un directorio raíz.
 *
 * Autor: Ka Fung (18-10492)
 * Fecha: 08/07/2020 
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "utilidades.h"

#define BUFSIZE 1

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
 * - Si n es positivo, rota hacia la derecha
 * - Si n es negativo, rota hacia la izquierda
 * 
 */
int roll_aux2(char *path, struct Args *args) { 
    char *buffer_n, *buffer;
    int filesize;  
    int n = args->n;
    int m = abs(n);

    /* Verifica que el archivo fue abierto */
    int fd = open(path, O_RDWR);
    if (fd == -1) return -1;

    filesize = lseek(fd, -1, SEEK_END);
    buffer_n = (char*)malloc(sizeof(char) * n);
    buffer = (char*)malloc(sizeof(char) * (filesize - m));
    if (!buffer_n || !buffer) return -1;

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

int roll_aux(char *path, struct Args *args) { 
    char *buffer_n, *buffer;
    int filesize, unread;  
    int n = args->n;

    /* Verifica que el archivo fue abierto */
    int fd = open(path, O_RDWR);
    if (fd == -1) return -1;

    filesize = lseek(fd, -1, SEEK_END);
    unread = filesize;
    buffer_n = (char*)malloc(abs(n));
    buffer = (char*)malloc(BUFSIZE);

    /* Si n es positivo, se rota hacia la derecha */
    if (n > 0) {
        /* Lee los últimos n caracteres */
        lseek(fd, -n-1, SEEK_END);
        read(fd, buffer_n, n);
        unread -= n;
        
        /* Rota bloques de BUFSIZE caracteres */
        while (unread >= BUFSIZE) {
            lseek(fd, unread - BUFSIZE, SEEK_SET);
            read(fd, buffer, BUFSIZE);
            lseek(fd, n - BUFSIZE, SEEK_CUR);
            write(fd, buffer, BUFSIZE);
            unread -= BUFSIZE;
        }

        /* Rota los caracteres restantes */
        lseek(fd, 0, SEEK_SET);
        read(fd, buffer, unread);
        lseek(fd, n, SEEK_SET);
        write(fd, buffer, unread);

        /* Escribe los n caracteres */
        lseek(fd, 0, SEEK_SET);
        write(fd, buffer_n, n);

    } else if (n < 0) {
        n = abs(n);
        /* Si n es negativo, se rota hacia la izquierda */
        /* Lee los primeros n caracteres */
        lseek(fd, 0, SEEK_SET);
        read(fd, buffer_n, n);
        unread -= n;
        
        /* Rota bloques de BUFSIZE caracteres */
        while (unread >= BUFSIZE) {
            lseek(fd, -unread-1, SEEK_END);
            read(fd, buffer, BUFSIZE);
            lseek(fd, -BUFSIZE - n, SEEK_CUR);
            write(fd, buffer, BUFSIZE);
            unread -= BUFSIZE;
        }
        
        /* Rota los caracteres restantes */
        lseek(fd, -unread-1, SEEK_END);
        read(fd, buffer, unread);
        lseek(fd, -unread - n, SEEK_CUR);
        write(fd, buffer, unread);
        
        /* Escribe los n caracteres */
        lseek(fd, -n-1, SEEK_END);
        write(fd, buffer_n, n);
    }
        

    free(buffer_n);
    close(fd);

    return 0;
}