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
 * Función que rota n caracteres el contenido de un archivo.
 * - Si n es positivo, rota hacia la derecha.
 * - Si n es negativo, rota hacia la izquierda.
 * 
 * Parámetros:
 *      path: ruta del archivo a rotar el contenido.
 *      args: puntero a una estructura Args que contiene 
 *            el número de caracteres a rotar.
 * Retorno:
 *      0 si todo fue correcto, -1 si hubo un error.
 */
 int roll_aux(char *path, struct Args *args) { 
    char *buffer_n, *buffer;
    int filesize, unread;  
    int n = args->n;

    /* Verifica que el archivo fue abierto */
    int fd = open(path, O_RDWR);
    if (fd == -1) return -1;

    filesize = lseek(fd, -1, SEEK_END);
    buffer_n = (char*)malloc(sizeof(char) * abs(n));
    buffer = (char*)malloc(sizeof(char) * BUFSIZE);
    if (!buffer_n || !buffer || filesize == -1) return -1;

    unread = filesize;
    /* Si n es positivo, se rota hacia la derecha */
    if (n > 0) {
        /* Lee los últimos n caracteres */
        if (lseek(fd, -n-1, SEEK_END) == -1) return -1;
        if (read(fd, buffer_n, n) == -1) return -1;
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
        /* Si n es negativo, se rota hacia la izquierda */
        n = abs(n);

        /* Lee los primeros n caracteres */
        lseek(fd, 0, SEEK_SET);
        read(fd, buffer_n, n);
        unread -= n;
        
        /* Rota bloques de BUFSIZE caracteres */
        while (unread >= BUFSIZE) {
            lseek(fd, -unread-1, SEEK_END);
            read(fd, buffer, BUFSIZE);
            lseek(fd, -BUFSIZE-n, SEEK_CUR);
            write(fd, buffer, BUFSIZE);
            unread -= BUFSIZE;
        }
        
        /* Rota los caracteres restantes */
        lseek(fd, -unread-1, SEEK_END);
        read(fd, buffer, unread);
        lseek(fd, -unread-n, SEEK_CUR);
        write(fd, buffer, unread);
        
        /* Escribe los n caracteres */
        lseek(fd, -n-1, SEEK_END);
        write(fd, buffer_n, n);
    }
        
    free(buffer_n);
    free(buffer);
    close(fd);

    return 0;
}