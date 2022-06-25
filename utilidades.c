/**
 * utilidades.c
 * 
 * Autor: Ka Fung (1810492)
 * Fecha: 08/07/2020 
 */
 
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include "utilidades.h"

#define BUFSIZE 4096

/**
 * Funcion que determina si un archivo es un directorio
 * 
 * Parámetros:
 *      path: ruta del archivo
 * Retorno:
 *      No nulo si el archivo es un directorio, NULL en caso contrario
 */
int is_dir_file(char *path) {
    struct stat st;
    if (stat(path, &st) == -1) {
        fprintf(stderr, "No se pudo aplicar stat sobre el archivo %s\n", path);
        return -1;
    }
    return S_ISDIR(st.st_mode);
}

/**
 * Función que revisa si el nombre del archivo contiene una cadena dada (case sensitive).
 * 
 * Parámetros:
 *      path: ruta del archivo
 *      string: cadena a buscar
 * Retorno:
 *      0 si no contiene la cadena, 1 si contiene la cadena
 */
int find(struct Args *args) {
    char *path = args->path;
    char *string = args->cadena1;
    if (strstr(path, string)) {
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
 *      string: cadena a buscar
 * Retorno:
 *      0 si no contiene la cadena, 1 si contiene la cadena
 */
int ifind(struct Args *args) {
    char *path = args->path;
    char *string = args->cadena1;
    if (strcasestr(path, string)) {
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
 *      string: cadena a buscar
 * Retorno:
 *      0 si no contiene la cadena, 1 si contiene la cadena
 */
int cfind(char *path, char *string, char *string2) {
    char *buffer;
    int fd;

    /* Verifica si path contiene el string */
    if (strstr(path, string)) return 0;
    
    /* Abre el archivo regular */
    fd = open(path, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "Error al abrir el archivo %s\n", path);
        return -1;
    }

    /* Verifica que el contenido del archivo tenga la cadena string2 */
    buffer = malloc(sizeof(char) * BUFSIZE);
    if (buffer == NULL) {
        fprintf(stderr, "Error al asignar memoria\n");
        return -1;
    }

    while (read(fd, buffer, BUFSIZE) > 0) {
        if (strstr(buffer, string2)) return 1;
    }

    free(buffer);

    close(fd);
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

    /* Por cada caracter, cuenta cuantos saltos de líneas y caracteres hay */
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
int wc(char *path, int *cum_lines, int *cum_chars) {
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
            if (is_dir == -1) return -1;

            if (is_dir) {
                /* Si es un directorio, sigue recorriendo */
                int status = wc(new_path, cum_lines, cum_chars);
                if (status == -1) return -1;
                lines += *cum_lines;
                chars += *cum_chars;
            } else {
                /* Si es un archivo regular, cuenta el número de líneas y caracteres */
                if (count_lines_chars(new_path, &lines, &chars) == -1) return -1;
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
 * Función que invierte el contenido de un archivo
 * 
 */
/* int codif2(char *path) {
    char inicio, fin;
    int filesize;
    int len1, len2, n;
    int fd1 = open(path, O_RDWR);
    int fd2 = open(path, O_RDWR);

    if (fd1 == -1) return -1;

    filesize = lseek(fd2, 0, SEEK_END) - 1;
    n = filesize / 2;

    while (n) {
        read(fd1, &inicio, 1);
        read(fd2, &fin, 1);

        lseek(fd1, -1, SEEK_CUR);
        lseek(fd2, -1, SEEK_CUR);

        write(fd1, &fin, 1);
        write(fd2, &inicio, 1);

        lseek(fd2, -2, SEEK_CUR);

        n--;
    }

    close(fd1);
    return 0;
}  */

/**
 * Función que invierte el contenido de un archivo
 * 
 * Parámetros:
 *      path: ruta del archivo a invertir el contenido
 * Retorno:
 *      0 si todo fue correcto, -1 si hubo un error
 */
int codif(char *path) {
    FILE *fp1;
    char inicio, fin;
    long m, n, filesize;

    /* Abre el archivo */
    fp1 = fopen(path, "r+");
    if (!fp1) return -1;

    /* Invierte el contenido del archivo */
    fseek(fp1, -1,  SEEK_END);
    filesize = ftell(fp1);

    m = 0;
    n = filesize / 2;
    while (n) {
        fseek(fp1, m++, SEEK_SET);
        inicio = fgetc(fp1);

        fseek(fp1, -m, SEEK_END);
        fin = fgetc(fp1);

        fseek(fp1, -m, SEEK_END);
        fprintf(fp1, "%c", inicio);

        fseek(fp1, m-1, SEEK_SET);
        fprintf(fp1, "%c", fin);

        n--;
    }

    fclose(fp1);
    return 0;
}

/**
 * Función que rota n caracteres el contenido de un archivo
 * Si n es positivo, rota hacia la derecha
 * Si n es negativo, rota hacia la izquierda
 * 
 */
/* int roll(char *path, int n) {
    int fd = open(path, O_RDONLY);
    int lines = 0;
    int chars = 0;
    char c;

    close(fd);
    return 0;
} */


/**
 * Función que recorre recursivamente 
 * 
 * Parámetros:
 *      fun: función a ejecutar por cada archivo
 *      args: argumentos para la función
 *      action_to_dir: indica si la función a ejecutar es para un directorio o un archivo
 *
 */
int traverseDir(char* path, int (*fun) (struct Args* argum), struct Args* argum, int action_to_dir) {
    DIR* dir;
    struct dirent* ent;

    /* Verifica que existe el directorio */
    if (!(dir = opendir(path))) {
        fprintf(stderr, "Error al abrir el directorio %s\n", path);
        return -1;
    } 
    
    while ((ent = readdir(dir))) {
        char* e_name = ent->d_name;
        int dots = strcmp(e_name, ".") == 0 || strcmp(e_name, "..") == 0;

        /* Concatena la nueva direccion */
        char* new_path = malloc(strlen(path) + strlen(e_name) + 2);
        strcpy(new_path, path);
        strcat(new_path, "/");
        strcat(new_path, e_name);

        /* Recorre recursivamente por cada directorio */
        if (!dots) {
            int is_dir = is_dir_file(new_path);
            if (is_dir == -1) return -1;

            /* Si es un directorio, sigue recorriendo */
            if (is_dir) {
                if (traverseDir(new_path, fun, argum, action_to_dir) == -1) return -1;
            } else {
                /* Si es un archivo regular,  */
                if (action_to_dir == 0) {
                    argum->path = new_path;
                    if (fun(argum) == -1) return -1;
                }
            }
        }
        free(new_path);
    }

    closedir(dir);
    return 0;
}
