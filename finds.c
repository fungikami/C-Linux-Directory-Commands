#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "finds.h"

#define BUFSIZE 1024

/**
 * Función que revisa si el nombre del archivo contiene una cadena dada (case sensitive).
 * 
 * Parámetros:
 *      path: ruta del archivo
 *      args: argumentos necesarios para la funcion 
 * Retorno:
 *      0 si no contiene la cadena, 1 si contiene la cadena
 */
int find(char *path, struct Args *args) {
    char *string = args->cadena1;
    if (!string || strstr(path, string)) {
        printf("%s\n", path);
        return 1;
    }
    return 0;
}

/**
 * Función que revisa si el nombre del archivo contiene una cadena dada (case insensitive).
 * 
 * Parámetros:
 *      path: ruta del archivo
 *      args: argumentos necesarios para la funcion 
 * Retorno:
 *      0 si no contiene la cadena, 1 si contiene la cadena
 */
int ifind(char *path, struct Args *args) {
    char *string = args->cadena1;
    if (!string || strcasestr(path, string)) {
        printf("%s\n", path);
        return 1;
    }
    return 0;
}

/**
 * Función que revisa si el nombre del archivo contiene una cadena dada (case sensitive).
 * y si el contenido contiene string2.
 * Parámetros:
 *      path: ruta del archivo
 *      args: argumentos necesarios para la funcion 
 * Retorno:
 *      0 si no contiene la cadena, 1 si contiene la cadena
 */
int cfind(char *path, struct Args *args) {
    char *string = args->cadena1;
    char *string2 = args->cadena2;
    char buffer[BUFSIZE];
    int fd, len, cur_char, i;

    /* Verifica si path contiene el string */
    if (!strstr(path, string)) return 0;
    
    /* Abre el archivo regular */
    fd = open(path, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "Error al abrir el archivo %s\n", path);
        return -1;
    }

    /* Verifica que el contenido del archivo tenga la cadena string2 */
    cur_char = 0;
    while ((len = read(fd, buffer, BUFSIZE)) > 0) {

        /* Por cada char del buffer, verifica si existe coincidencia de char */
        for (i = 0; i < len; i++) {
            if (buffer[i] == string2[cur_char]) {
                cur_char++;
                if (cur_char == strlen(string2)) {
                    printf("%s\n", path);
                    return 1;
                }
            } else {
                cur_char = 0;
            }
        }
    }

    close(fd);
    return 0;
}