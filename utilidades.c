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

#define BUFSIZE 1048

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
 * Funcion que determina si un archivo es regular
 * 
 * Parámetros:
 *      path: ruta del archivo
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
    int fd, len;

    /* Verifica si path contiene el string */
    if (!strstr(path, string)) return 0;
    
    /* Abre el archivo regular */
    fd = open(path, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "Error al abrir el archivo %s\n", path);
        return -1;
    }

    /* Verifica que el contenido del archivo tenga la cadena string2 */
    while ((len = read(fd, buffer, BUFSIZE)) > 0) {
        if (strstr(buffer, string2)) {
            printf("%s\n", path);
            return 1;
        }
    }

    close(fd);
    return 0;
}

int cfind2(char *path, struct Args *args) {
    FILE *stream;
    char *line = NULL;
    char *string = args->cadena1;
    char *string2 = args->cadena2;
    size_t len = 0;
    ssize_t nread;

    /* Verifica si path contiene el string */
    if (!strstr(path, string)) return 0;
    
    /* Abre el archivo regular */
    stream = fopen(path, "r");
    if (stream == NULL) {
        fprintf(stderr, "Error al abrir el archivo %s\n", path);
        return -1;
    }

    /* Verifica que el contenido del archivo tenga la cadena string2 */
    while ((nread = getline(&line, &len, stream)) != -1) {
        if (strstr(line, string2)) {
            printf("%s\n", path);
            return 1;
        }
    }

    free(line);
    fclose(stream);
    return 0;
}


int repla(char *path, struct Args *args) {
    /* lista_cabeza = extraer_palabras(argv[1]);
    if (!lista_cabeza) {
        fprintf(stderr, "Error al extraer palabras\n");
        return -1;
    }

    for (i = 2; i < argc; i++) {
        int j = remplazar_palabras(argv[i], lista_cabeza);
        if (j < 0) {
            fprintf(stderr, "Error al reemplazar palabras\n");
            return -1;
        }
        
    } */
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
        lseek(fd, m++, SEEK_SET);
        read(fd, &izq, 1);

        /* Lee el caracter más derecho a intercambiar */
        lseek(fd, -m, SEEK_END);
        read(fd, &der, 1);

        /* Escribe el caracter más derecho en la izquierda*/
        lseek(fd, -m, SEEK_END);
        write(fd, &izq, 1);

        /* Escribe el caracter más izquierdo en la derecha*/
        lseek(fd, m-1, SEEK_SET);
        write(fd, &der, 1);

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
