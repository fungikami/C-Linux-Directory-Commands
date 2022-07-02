/**
 * codif.c
 * Implementación del comando codif, en la cual cada archivo regular, 
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
#include "utilidades.h"

int codif_aux(char *path, void *args);

/**
 * Función que llama la función auxiliar codif_aux
 * para invertir el contenido de los archivos de un directorio raíz.
 * 
 * Parámetros:
 *      directorioRaiz: ruta del directorio raíz 
 */
void codif(char *directorioRaiz) {
    if (traverseDir(directorioRaiz, codif_aux, NULL, 0) == -1) {
        fprintf(stderr, "Error al ejecutar codif.\n");
    }
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
    char *buffer1, *buffer2;
    int fd = open(path, O_RDWR), unread, n;

    /* Verifica que el archivo fue abierto */
    if (fd == -1) return -1;

    /* Reserva memoria para los bloques a intercambiar */
    buffer1 = (char*)malloc(sizeof(char) * BUFSIZ);
    buffer2 = (char*)malloc(sizeof(char) * BUFSIZ);
    if (!buffer1 || !buffer2) {
        free(buffer1);
        free(buffer2);
        close(fd);
        return -1;
    }

    /* Determinar mitad del archivo */
    n = lseek(fd, 0, SEEK_END) / 2;
    lseek(fd, 0, SEEK_SET);

    unread = n;
    while (unread) {
        /* Determinar el tamaño del bloque a leer */
        int i, offset = n - unread, toread = unread;
        if (toread >= BUFSIZ) toread = BUFSIZ;
        
        /* Lee el bloque más izquierdo a intercambiar */
        lseek(fd, offset, SEEK_SET);
        read(fd, buffer1, toread);

        /* Lee el bloque más derecho a intercambiar */
        lseek(fd, -toread-offset, SEEK_END);
        read(fd, buffer2, toread);

        /* Intercambia el contenido de los bloques */
        for (i = 0; i < toread; i++) {
            char temp = buffer1[i];
            buffer1[i] = buffer2[toread - i - 1];
            buffer2[toread - i - 1] = temp;
        }

        /* Escribe el bloque intercambiado más izquierdo */
        lseek(fd, offset, SEEK_SET);
        write(fd, buffer1, toread);

        /* Escribe el bloque intercambiado más derecho */
        lseek(fd, -toread-offset, SEEK_END);
        write(fd, buffer2, toread);
        
        unread -= toread;
    }

    /* Cierra el archivo */
    close(fd);
    return 0;
}