/**
 * utilidades.c
 * 
 * Autor: Ka Fung (18-10492)
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

#define BUFSIZE 1024

/** 
 * Extrae la línea de comando de manera dinámica.
 * Retorno:
 *      char* con la línea de comando
 */
char *get_line() {
    int n = 100, i = 0;
    char c = fgetc(stdin);
    char *line = malloc(sizeof(char) * n);
    if (!line) return NULL;
    
    /* Obtiene caracter por caracter del stdin */
    while (c != '\n' && c != EOF) {
        line[i] = c;
        i++;
        c = fgetc(stdin);

        /* Si aún queda por scannear pero no hay espacio, realloc */
        if (i == n) {
            line = realloc(line, sizeof(char) * (n + 100));
            if (!line) {
                free(line);
                return NULL;
            }
            n += 100;
        }
    } 

    /* Realloc ajustado el tamaño de la linea */
    line = realloc(line, sizeof(char) * (i + 1));
    if (!line) {
        free(line);
        return NULL;
    }
    line[i] = '\0';
    return line;
}

/**
 * Funcion que determina si un archivo es un directorio
 * 
 * Parámetros:
 *      path: ruta del archivo
 * Retorno:
 *      No nulo es una archivo de directorio, NULL en caso contrario
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
 * Funcion que determina si un archivo es regular
 * 
 * Parámetros:
 *      path: ruta del archivo
 * Retorno:
 *      No nulo si es un archivo regular, NULL en caso contrario
 */
int is_reg_file(char *path) {
    struct stat st;
    if (stat(path, &st) == -1) {
        fprintf(stderr, "No se pudo aplicar stat sobre el archivo %s\n", path);
        return -1;
    }

    return S_ISREG(st.st_mode);
}  

/**
 * Función que recorre recursivamente 
 * 
 * Parámetros:
 *      fun: función a ejecutar por cada archivo
 *      args: argumentos para la función
 *      action_to_dir: indica si la función a ejecutar es para un directorio y/o un archivo
 *      0 si es solo un archivo, 1 si es un directorio, 2 si es un archivo y un directorio
 * Retorno:
 *      0 si todo fue correcto, -1 si hubo un error
 */
int traverseDir(char* path, int (*fun) (char *path, struct Args* argum), struct Args* argum, int action_to_dir) {
    DIR* dir;
    struct dirent* ent;

    /* Verifica que existe el directorio */
    if (!(dir = opendir(path))) {
        fprintf(stderr, "Error al abrir el directorio %s\n", path);
        return -1;
    } 
    
    /* Recorre el directorio */
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
            if (is_dir == -1) {
                free(new_path);
                continue;
            }

            if (is_dir) {
                /* Si es un directorio, sigue recorriendo */
                if (traverseDir(new_path, fun, argum, action_to_dir) == -1) {
                    free(new_path);
                    continue;
                }
            } else {
                int is_reg = is_reg_file(new_path);
                if (is_reg == -1) return -1;

                /* Si es un archivo regular, llama la función */
                if (is_reg) {
                    if (action_to_dir==0 || action_to_dir==2) {
                        if (fun(new_path, argum) == -1) {
                            free(new_path);
                            continue;
                        }
                    }
                }
            }
        }

        free(new_path);
    }

    /* Si trata de una función a ejecutar en directorios */
    if (action_to_dir==1 || action_to_dir==2) {
        if (fun(path, argum) == -1) {
            closedir(dir);
            return -1;
        }
    }

    closedir(dir);
    return 0;
}
