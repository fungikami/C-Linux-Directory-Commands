/**
 * codif.c
 * Implementación del comando codif, en la cual para cada archivo regular 
 * que se encuentre en el directorio raíz, se invierte su contenido.
 *
 * Autor: Ka Fung (18-10492)
 * Fecha: 08/07/2020 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "misc.h"

int codif_aux(char *path, void *args);

/**
 * Función que llama la función auxiliar codif_aux para 
 * invertir el contenido de los archivos de un directorio raíz.
 * 
 * Parámetros:
 *      dir_raiz: ruta del directorio raíz 
 */
void codif(char *dir_raiz) {
    if (traverse_dir(dir_raiz, codif_aux, NULL, 0) == -1)
        fprintf(stderr, "Error al ejecutar codif.\n");
}

/**
 * Función que invierte el contenido de un archivo, es decir, 
 * intercambia el primer y último carácter, el segundo y penúltimo, 
 * y así hasta que todos los caracteres hayan sido intercambiados.
 * 
 * Parámetros:
 *      path: ruta del archivo a invertir el contenido
 * Retorno:
 *      0 si todo fue correcto, -1 si hubo un error
 */
int codif_aux(char *path, void *args) {
    char *lbuffer, *rbuffer;
    int fd = open(path, O_RDWR), unread, n;

    /* Verifica que el archivo fue abierto */
    if (fd == -1) return -1;

    /* Reserva memoria para los bloques a intercambiar */
    lbuffer = (char*)malloc(sizeof(char) * BUFSIZ);
    rbuffer = (char*)malloc(sizeof(char) * BUFSIZ);
    if (!lbuffer || !rbuffer) {
        free(lbuffer);
        free(rbuffer);
        close(fd);
        return -1;
    }

    /* Determinar mitad del archivo */
    n = lseek(fd, 0, SEEK_END) / 2;
    if (lseek(fd, 0, SEEK_SET) == -1 || n == -1) {
        free(lbuffer);
        free(rbuffer);
        close(fd);
        return -1;
    }

    unread = n;
    while (unread) {
        /* Determinar el tamaño del bloque a leer */
        int i, lpos, rpos, toread = unread; 
        if (toread >= BUFSIZ) toread = BUFSIZ;

        /* Determinar la posición del puntero */
        lpos = n-unread;
        rpos = -toread-lpos;
        
        /* Lee el bloque más izquierdo a intercambiar */
        if (lseek(fd, lpos, SEEK_SET) == -1 || read(fd, lbuffer, toread) == -1) {
            free(lbuffer);
            free(rbuffer);
            close(fd);
            return -1;
        }

        /* Lee el bloque más derecho a intercambiar */
        if (lseek(fd, rpos, SEEK_END) == -1 || read(fd, rbuffer, toread) == -1) {
            free(lbuffer);
            free(rbuffer);
            close(fd);
            return -1;
        }

        /* Intercambia el contenido de los bloques */
        for (i = 0; i < toread; i++) {
            char temp = lbuffer[i];
            lbuffer[i] = rbuffer[toread - i - 1];
            rbuffer[toread - i - 1] = temp;
        }

        /* Escribe el bloque intercambiado más izquierdo */
        if (lseek(fd, lpos, SEEK_SET) == -1 || write(fd, lbuffer, toread) == -1) {
            free(lbuffer);
            free(rbuffer);
            close(fd);
            return -1;
        }

        /* Escribe el bloque intercambiado más derecho */
        if (lseek(fd, rpos, SEEK_END) == -1 || write(fd, rbuffer, toread) == -1) {
            free(lbuffer);
            free(rbuffer);
            close(fd);
            return -1;
        }
        
        unread -= toread;
    }

    /* Cierra el archivo y libera memoria */
    free(lbuffer);
    free(rbuffer);
    close(fd);
    return 0;
}