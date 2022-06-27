#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "utilidades.h"

#define BUFSIZE 1024

int find_aux(char *path, struct Args *args);
int ifind_aux(char *path, struct Args *args);
int cfind_aux(char *path, struct Args *args);

/**
 * Función que llama la función auxiliar find_aux 
 * para buscar los nombres de archivos de un directorio raíz
 * que contienen la cadena dada.
 * 
 * Parámetros:
 *      directorioRaiz: ruta del directorio raíz 
 */
void find(char *directorioRaiz, char *cadena) {
    struct Args* args = (struct Args*)malloc(sizeof(struct Args));
    if (!args) {
        fprintf(stderr, "Error al reservar memoria\n");
    }
    args->cadena1 = cadena;
    if (traverseDir(directorioRaiz, find_aux, args, 2) == -1) {
        fprintf(stderr, "Error al ejecutar ifind con el archivo %s\n", directorioRaiz);
    }
    free(args);
}

/**
 * Función que revisa si el nombre del archivo contiene una cadena dada (case sensitive).
 * 
 * Parámetros:
 *      path: ruta del archivo
 *      args: argumentos necesarios para la funcion 
 * Retorno:
 *      0 si no contiene la cadena, 1 si contiene la cadena
 */
int find_aux(char *path, struct Args *args) {
    char *string = args->cadena1;
    if (!string || strstr(path, string)) {
        printf("%s\n", path);
        return 1;
    }
    return 0;
}

/**
 * Función que llama la función auxiliar ifind_aux 
 * para buscar los nombres de archivos de un directorio raíz
 * que contienen la cadena dada (no case sensitive).
 * 
 * Parámetros:
 *      directorioRaiz: ruta del directorio raíz 
 */
void ifind(char *directorioRaiz, char *cadena) {
    struct Args* args = (struct Args*)malloc(sizeof(struct Args));
    if (!args) {
        fprintf(stderr, "Error al reservar memoria\n");
    }
    args->cadena1 = cadena;
    traverseDir(directorioRaiz, ifind_aux, args, 2);
    free(args);
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
int ifind_aux(char *path, struct Args *args) {
    char *string = args->cadena1;
    if (!string || strcasestr(path, string)) {
        printf("%s\n", path);
        return 1;
    }
    return 0;
}

/**
 * Función que llama la función auxiliar cfind_aux 
 * para buscar los nombres de archivos de un directorio raíz que 
 * contienen la cadena1 dada y que su contenido contenga la cadena2 .
 * 
 * Parámetros:
 *      directorioRaiz: ruta del directorio raíz 
 */
void cfind(char *directorioRaiz, char *cadena1, char *cadena2) {
    struct Args* args = (struct Args*)malloc(sizeof(struct Args));
    if (!args) {
        fprintf(stderr, "Error al reservar memoria\n");
    }

    args->cadena1 = cadena1;
    args->cadena2 = cadena2;
    traverseDir(directorioRaiz, cfind_aux, args, 2);
    free(args);
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
int cfind_aux(char *path, struct Args *args) {
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