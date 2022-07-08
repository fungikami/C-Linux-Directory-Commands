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

int roll_aux(char *path, void *args);

/**
 * Función que llama la función auxiliar roll para rotar n  
 * caracteres del contenido de los archivos de un directorio raíz.
 * 
 * Parámetros:
 *      dir_raiz: ruta del directorio raíz 
 *      n: número de caracteres a rotar
 */
void roll(char *dir_raiz, int n) {
    if (traverse_dir(dir_raiz, roll_aux, &n, 0) == -1)
        fprintf(stderr, "Error al ejecutar roll.\n");
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
 int roll_aux(char *path, void *args) { 
    char *nbuffer, *buffer;
    int unread, n = *(int *) args;

    /* Verifica que el archivo fue abierto */
    int fd = open(path, O_RDWR);
    if (fd == -1) return -1;

    /* Reserva memoria para los bloques a rotar */
    nbuffer = (char*)malloc(sizeof(char) * abs(n));
    buffer = (char*)malloc(sizeof(char) * BUFSIZ);
    unread = lseek(fd, 0, SEEK_END);
    if (!nbuffer || !buffer || unread == -1) {
        free(nbuffer);
        free(buffer);
        close(fd);
        return -1;
    }

    /* Si n es positivo, se rota hacia la derecha */
    if (n > 0) {
        /* Lee los últimos n caracteres */
        if (lseek(fd, -n, SEEK_END) == -1 || read(fd, nbuffer, n) == -1) {
            free(nbuffer);
            free(buffer);
            close(fd);
            return -1;
        }
        unread -= n;
        
        /* Rota bloques de BUFSIZ caracteres */
        while (unread) {
            /* Determinar el tamaño del bloque a leer */
            int toread = unread;
            if (unread > BUFSIZ) toread = BUFSIZ;

            /* Lee el bloque más derecho a rotar */
            if (lseek(fd, unread - toread, SEEK_SET) == -1 || read(fd, buffer, toread) == -1) {
                return -1;
            }

            /* Escribe el bloque más derecho a rotar */
            if (lseek(fd, n - toread, SEEK_CUR) == -1 || write(fd, buffer, toread) == -1) {
                return -1;
            }

            unread -= toread;
        }

        /* Escribe los n caracteres */
        if (lseek(fd, 0, SEEK_SET) == -1 || write(fd, nbuffer, n) == -1) {
            free(nbuffer);
            free(buffer);
            close(fd);
            return -1;
        }

    } else if (n < 0) {
        /* Si n es negativo, se rota hacia la izquierda */
        n = abs(n);

        /* Lee los primeros n caracteres */
        if (lseek(fd, 0, SEEK_SET) == -1 || read(fd, nbuffer, n) == -1) {
            free(nbuffer);
            free(buffer);
            close(fd);
            return -1;
        }
        unread -= n;
        
        /* Rota bloques de BUFSIZ caracteres */
        while (unread) {
            int toread = unread;
            if (unread > BUFSIZ) toread = BUFSIZ;

            if (lseek(fd, -unread, SEEK_END) == -1 || read(fd, buffer, toread) == -1) {
                return -1;
            }

            if (lseek(fd, -toread-n, SEEK_CUR) == -1 || write(fd, buffer, toread) == -1) {
                return -1;
            }
            
            unread -= toread;
        }
        
        /* Escribe los n caracteres */
        if (lseek(fd, -n, SEEK_END) == -1 || write(fd, nbuffer, n) == -1) {
            free(nbuffer);
            free(buffer);
            close(fd);
            return -1;
        }
    }
        
    free(nbuffer);
    free(buffer);
    close(fd);

    return 0;
}