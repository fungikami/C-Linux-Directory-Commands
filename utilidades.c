/**
 * utilidades.c
 * Implementación de diversas funciones que se harán uso durnate el programa.
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

/**
 * Función que remueve los comillas de una cadena.
 * Parámetros:
 *  - str: cadena de caracteres a la cual se le removeran las comillas.
 */
void remove_quotes(char *str) {
    if (str && str[0] == '\"' && str[strlen(str) - 1] == '\"') {
        int i;
        for (i = 0; i < strlen(str); i++) {
            str[i] = str[i + 1];
        }
        str[strlen(str) - 1] = '\0';
    }
}

/**
 * Función que separa una cadena de caracteres en dos cadenas. 
 * En caso de tener comillas, las remueve.
 * Parámetros:
 *  - str: cadena de caracteres a la cual se le separara.
 *  - cadena1: primera cadena de caracteres que se guardara.
 *  - cadena2: segunda cadena de caracteres que se guardara.
 * Retorno:
 *  - 0: si la separación se realizo correctamente, -1 en caso contrario.
 */
int separate_str(char *str, char **cadena1, char **cadena2) {
    int i;

    if (!str) return -1;

    /* Reserva memoria para las dos cadenas */
    *cadena1 = (char *) malloc(sizeof(char) * (strlen(str) + 1));
    *cadena2 = (char *) malloc(sizeof(char) * (strlen(str) + 1));
    if (!*cadena1 || !*cadena2) {
        free(*cadena1);
        free(*cadena2);
        return -1;
    }

    if (str[0] == '\"') {
        /* Si hay una comilla al principio, guarda en cadena1 hasta la otra comilla */
        for (i = 1; str[i] != '\"' && str[i] != '\0'; i++) {
            (*cadena1)[i - 1] = str[i];
        }
        (*cadena1)[i - 1] = '\0';
        i++;
    } else {
        /* Si no hay comilla al principio, guarda en cadena1 hasta el espacio */
        for (i = 0; str[i] != ' ' && str[i] != '\0'; i++) {
            (*cadena1)[i] = str[i];
        }
        (*cadena1)[i] = '\0';
    } 

    /* Verifica que exista una cadena2 y lo copia sin comillas */
    if (str[i] == '\0') {
        free(*cadena1);
        free(*cadena2);
        return -1;
    }
    strcpy(*cadena2, str + i + 1);
    remove_quotes(*cadena2);

    return 0;
}

/** 
 * Extrae la línea de comando de manera dinámica.
 * Retorno:
 *      char* con la línea de comando
 */
char *get_line() {
    int n = 64, i = 0;
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
            line = realloc(line, sizeof(char) * (n + 64));
            if (!line) {
                free(line);
                return NULL;
            }
            n += 64;
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
 * Función que verifica si una cadena es un entero.
 * Parámetros:
 *      str: cadena a verificar
 * Retorno:
 *      1 si la cadena es un entero, 0 si no lo es
 */
int is_integer(char *str) {
    int i = 0;
    if (str[i] == '-' || str[i] == '+') i++;
    for (; str[i]; i++) {
        if (!isdigit(str[i])) return 0;
    }
    return 1;
}

/**
 * Función que determina si un archivo es un directorio.
 * 
 * Parámetros:
 *      path: ruta del archivo.
 * Retorno:
 *      No nulo es una archivo de directorio, NULL en caso contrario.
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
 * Función que determina si un archivo es regular.
 * 
 * Parámetros:
 *      path: ruta del archivo.
 * Retorno:
 *      No nulo si es un archivo regular, NULL en caso contrario.
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
 * Función que recorre recursivamente desde un directorio dado y ejecuta una función indicada.
 * 
 * Parámetros:
 *      fun: función a ejecutar por cada archivo.
 *      args: argumentos de la función.
 *      action: indica si la función a ejecutar es para un directorio y/o un archivo
 *          (0 si es para regulares, 1 si es para directorios, 2 si es para ambos casos)
 * Retorno:
 *      0 si todo fue correcto, -1 si hubo un error durante la ejecución.
 */
int traverse_dir(char* path, int (*fun) (char *, void *), void *argum, int action) {
    DIR* dir;
    struct dirent* ent;

    /* Verifica que existe el directorio */
    if (!(dir = opendir(path))) {
        fprintf(stderr, "Error al abrir el directorio %s\n", path);
        return -1;
    } 
    
    /* Recorre el contenido del directorio */
    while ((ent = readdir(dir))) {
        char* e_name = ent->d_name;
        int dots = strcmp(e_name, ".") == 0 || strcmp(e_name, "..") == 0;

        /* Concatena la nueva direccion */
        char* new_path = malloc(sizeof(char) * (strlen(path) + strlen(e_name) + 2));
        strcpy(new_path, path);
        strcat(new_path, "/");
        strcat(new_path, e_name);

        /* Se revisa el contenido del archivo, evitando aquellos que terminen en '.' o '..' */
        if (!dots) {
            int is_dir = is_dir_file(new_path);
            if (is_dir == -1) {
                free(new_path);
                continue;
            }

            if (is_dir) {
                /* Si es un directorio, se sigue recorriendo recursivamente */
                if (traverse_dir(new_path, fun, argum, action) == -1) {
                    free(new_path);
                    continue;
                }
            } else {
                int is_reg = is_reg_file(new_path);
                if (is_reg == -1) return -1;

                /* Si es un archivo regular, llama la función */
                if (is_reg && (action==0 || action==2)) {
                    if (fun(new_path, argum) == -1) {
                        free(new_path);
                        continue;
                    }
                }
            }
        }
        free(new_path);
    }

    /* Si trata de una función a ejecutar en directorios */
    if (action==1 || action==2) {
        if (fun(path, argum) == -1) {
            closedir(dir);
            return -1;
        }
    }

    closedir(dir);
    return 0;
}
