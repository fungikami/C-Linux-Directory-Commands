#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include "utilidades.h"

#define BUFSIZE 1024

int wc_aux(char *path, int *nlines, int *nchars);
int count_lines_chars(char *path, int *lines, int *chars);

/**
 * Función que llama la función auxiliar wc_aux 
 * para contar las lineas y caracteres desde un directorio raíz.
 * 
 * Parámetros:
 *      directorioRaiz: ruta del directorio raíz 
 */
void wc(char *directorioRaiz) {
    int lineas, chars;
    if (wc_aux(directorioRaiz, &lineas, &chars) == -1) {
        fprintf(stderr, "Error al ejecutar wc.\n");
    }
}

/**
 * Función que cuenta el total de lineas y caracteres de un archivo
 * 
 * Parámetros:
 *      path: ruta del archivo
 *      cum_lines: puntero a la variable que almacena el número acumulado de lineas
 *      cum_chars: puntero a la variable que almacena el número acumulado de caracteres
 * Retorno:
 *      0 si todo fue correcto, -1 si hubo un error
 */
int wc_aux(char *path, int *cum_lines, int *cum_chars) {
    DIR* dir;
    struct dirent* ent;
    int lines = 0, chars = 0;

    /* Verifica que existe el directorio */
    if (!(dir = opendir(path))) {
        fprintf(stderr, "Error al abrir el directorio %s\n", path);
        return -1;
    } 
    
    /* Recorre recursivamente por cada directorio */
    while ((ent = readdir(dir))) {
        char* e_name = ent->d_name;
        int dots = strcmp(e_name, ".") == 0 || strcmp(e_name, "..") == 0;

        /* Concatena la nueva direccion */
        char* new_path = malloc(strlen(path) + strlen(e_name) + 2);
        strcpy(new_path, path);
        strcat(new_path, "/");
        strcat(new_path, e_name);

        /* Si no se trata de una dirección con puntos, se revisa su contenido*/
        if (!dots) {
            int is_dir = is_dir_file(new_path);
            if (is_dir == -1) {
                free(new_path);
                continue;
            }

            if (is_dir) {
                /* Si es un directorio, sigue recorriendo */
                int status = wc_aux(new_path, cum_lines, cum_chars);
                if (status == -1) {
                    free(new_path);
                    continue;
                }
                lines += *cum_lines;
                chars += *cum_chars;
            } else {
                /* Si es un archivo regular, cuenta el número de líneas y caracteres */
                if (count_lines_chars(new_path, &lines, &chars) == -1) {
                    free(new_path);
                    continue;
                }
            }
        }

        free(new_path);
    }
    
    /* Imprime el número de líneas y caracteres del directorio actual */
    printf("El directorio %s: %d líneas y %d caracteres\n", path, lines, chars);
    
    /* Actualiza el número acumulado de líneas */
    (*cum_lines) = lines;
    (*cum_chars) = chars;

    closedir(dir);
    return 0;
}

/**
 * Función que cuenta el total de lineas y caracteres de un archivo
 * 
 * Parámetros:
 *      path: ruta del archivo
 *      lines: puntero a la variable que almacena el total de lineas
 *      chars: puntero a la variable que almacena el total de caracteres
 * Retorno:
 *      0 si todo fue correcto, -1 si hubo un error
 */
int count_lines_chars(char *path, int *lines, int *chars) {
    char c, buf[BUFSIZE];
    int len, i;
    int fd = open(path, O_RDONLY);

    /* Abre el archivo */
    if (fd == -1) {
        fprintf(stderr, "Error al abrir el archivo %s\n", path);
        return -1;
    }

    /* Cuenta cuantos saltos de líneas y caracteres hay por cada bloque */
    while ((len = read(fd, buf, BUFSIZE)) > 0) {
        for (i = 0; i < len; i++) {
            c = buf[i];
            if (c == '\n') {
                (*lines)++;
            }
            (*chars)++;
        }
    }

    /* Cierra el archivo */
    close(fd);
    return 0;
}
