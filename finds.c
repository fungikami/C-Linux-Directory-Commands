/**
 * finds.c
 * Implementación de los comandos find, ifind y cfind, en la cual cada imprime los
 * los archivos que tienen en su nombre una cadena dada. En caso de ser ifind, es 
 * no case sensitive. En caso de ser cfind, revisa que el contenido del archivo 
 * tenga la segunda cadena dada.
 *
 * Autor: Ka Fung (18-10492)
 * Fecha: 08/07/2020 
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "utilidades.h"

int find_aux(char *path, void *args);
int ifind_aux(char *path, void *args);
int cfind_aux(char *path, void *args);

/**
 * Función que llama la función auxiliar find_aux para buscar los nombres de 
 * archivos de un directorio raíz que contienen la cadena dada (case sensitive).
 * 
 * Parámetros:
 *      directorioRaiz: ruta del directorio raíz.
 */
void find(char *directorioRaiz, char *cadena) {
    if (traverseDir(directorioRaiz, find_aux, cadena, 2) == -1) {
        fprintf(stderr, "Error al ejecutar ifind con el archivo %s\n", directorioRaiz);
    }
}

/**
 * Función que revisa si el nombre del archivo contiene una cadena dada (case sensitive).
 * 
 * Parámetros:
 *      path: ruta del archivo.
 *      args: cadena a buscar en el nombre del archivo.
 * Retorno:
 *      0 si no contiene la cadena, 1 si contiene la cadena.
 */
int find_aux(char *path, void *args) {
    char *string = (char *) args;
    if (!string || strstr(path, string)) {
        printf("%s\n", path);
        return 1;
    }
    return 0;
}

/**
 * Función que llama la función auxiliar ifind_aux para buscar los nombres de 
 * archivos de un directorio raíz que contienen la cadena dada (no case sensitive).
 * 
 * Parámetros:
 *      directorioRaiz: ruta del directorio raíz 
 */
void ifind(char *directorioRaiz, char *cadena) {
    if (traverseDir(directorioRaiz, ifind_aux, cadena, 2) == -1) {
        fprintf(stderr, "Error al ejecutar ifind con el archivo %s\n", directorioRaiz);
    }
}

/**
 * Función que revisa si el nombre del archivo contiene una cadena dada (no case sensitive).
 * 
 * Parámetros:
 *      path: ruta del archivo.
 *      args: cadena a buscar en el nombre del archivo.
 * Retorno:
 *      0 si no contiene la cadena, 1 si contiene la cadena.
 */
int ifind_aux(char *path, void *args) {
    char *string = (char *) args;
    if (!string || strcasestr(path, string)) {
        printf("%s\n", path);
        return 1;
    }
    return 0;
}

/**
 * Función que llama la función auxiliar cfind_aux para buscar 
 * los nombres de archivos de un directorio raíz que contienen 
 * la cadena1 dada y que su contenido contenga la cadena2 .
 * 
 * Parámetros:
 *      directorioRaiz: ruta del directorio raíz 
 */
void cfind(char *directorioRaiz, char *cadena1, char *cadena2) {
    char **args = malloc(sizeof(char *) * 2);
    args[0] = cadena1;
    args[1] = cadena2;

    if (traverseDir(directorioRaiz, cfind_aux, args, 2) == -1) {
        fprintf(stderr, "Error al ejecutar cfind con el archivo %s\n", directorioRaiz);
    }

    free(args);
}

/**
 * Función que revisa si el nombre del archivo contiene una cadena dada 
 * (case sensitive) y verifica si el contenido del archivo contiene string2.
 *
 * Parámetros:
 *      path: ruta del archivo
 *      args: puntero de punteros, donde args[0] es la cadena1 y args[1] es la cadena2 
 * Retorno:
 *      0 si no contiene la cadena, 1 si contiene la cadena, -1 si hubo un error
 */
int cfind_aux(char *path, void *args) {
    char **string = (char **) args;
    char *string1 = string[0];
    char *string2 = string[1];
    char buffer[BUFSIZ];
    int fd, len, cur_char, i;

    /* Verifica si path contiene el string */
    if (!strstr(path, string1)) return 0;
    
    /* Abre el archivo regular */
    fd = open(path, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "Error al abrir el archivo %s\n", path);
        return -1;
    }

    /* Verifica que el contenido del archivo tenga la cadena string2 */
    cur_char = 0;
    while ((len = read(fd, buffer, BUFSIZ)) > 0) {

        /* Por cada char del buffer, verifica si existe coincidencia de char */
        for (i = 0; i < len; i++) {
            if (buffer[i] == string2[cur_char]) {
                cur_char++;
                if (cur_char == strlen(string2)) {
                    printf("%s\n", path);
                    close(fd);
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