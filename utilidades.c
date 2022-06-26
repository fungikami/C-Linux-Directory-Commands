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

#define BUFSIZE 1024

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
 * Función que invierte el contenido de un archivo
 * 
 * Parámetros:
 *      path: ruta del archivo a invertir el contenido
 * Retorno:
 *      0 si todo fue correcto, -1 si hubo un error
 */
int codif(char *path, struct Args *args) {
    int izq, der;
    int fd = open(path, O_RDWR);
    long m, n, filesize;

    /* Verifica que el archivo fue abierto */
    if (fd == -1) return -1;

    filesize = lseek(fd, -1, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    m = 0;
    n = filesize / 2;
    while (n) {
        /* Lee el caracter más izquierdo a intercambiar */
        if (lseek(fd, m++, SEEK_SET) == -1) return -1;
        if (read(fd, &izq, 1) == -1) return -1;

        /* Lee el caracter más derecho a intercambiar */
        if (lseek(fd, -m, SEEK_END) == -1) return -1;
        if (read(fd, &der, 1) == -1) return -1;

        /* Escribe el caracter más derecho en la izquierda*/
        if (lseek(fd, -m, SEEK_END) == -1) return -1;
        if (write(fd, &izq, 1) == -1) return -1;

        /* Escribe el caracter más izquierdo en la derecha*/
        if (lseek(fd, m-1, SEEK_SET) == -1) return -1;
        if (write(fd, &der, 1) == -1) return -1;

        n--;
    }

    /* Cierra el archivo */
    close(fd);
    return 0;
} 

/**
 * Función que rota n caracteres el contenido de un archivo
 * Si n es positivo, rota hacia la derecha
 * Si n es negativo, rota hacia la izquierda
 * 
 */
int roll(char *path, int n) {
    int fd = open(path, O_RDWR);
    int len1, len2, i;
    char *buf = malloc(n);
    char *buf_roll = malloc(n);
    read(fd, buf_roll, n);

    while ((len1 = read(fd, buf, n)) > 0) {
        printf("%s %d\n", buf, len1);
        len2 = 0;
        do {

            lseek(fd, -len1-n, SEEK_CUR);
            i = write(fd, buf + len2, len1 - len2);
            printf("%d\n", i);
            len2 += i;
            lseek(fd, len2+(len1-len2), SEEK_CUR);

        } while (len2 < len1);
    }

    close(fd);
    return 0;
}

/**
 * Función que recorre recursivamente 
 * 
 * Parámetros:
 *      fun: función a ejecutar por cada archivo
 *      args: argumentos para la función
 *      action_to_dir: indica si la función a ejecutar es para un directorio o un archivo
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

            if (is_dir) {
                /* Si es un directorio, sigue recorriendo */
                if (traverseDir(new_path, fun, argum, action_to_dir) == -1) return -1;
            } else {
                int is_reg = is_reg_file(new_path);
                if (is_reg == -1) return -1;

                /* Si es un archivo regular, llama la función */
                if (is_reg) {
                    if (!action_to_dir) {
                        if (fun(new_path, argum) == -1) return -1;
                    }
                }
            }
        }

        if (action_to_dir) {
            if (fun(new_path, argum) == -1) return -1;
        }

        free(new_path);
    }

    closedir(dir);
    return 0;
}
