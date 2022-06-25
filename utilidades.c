/**
 * utilidades.c
 * 
 * Autor: Ka Fung (1810492)
 * Fecha: 08/07/2020 
 */

#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

#define BUFSIZE 4096

/**
 * Funcion que determina si un archivo es un directorio
 * 
 * Parámetros:
 *      path: ruta del archivo
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
/* int is_reg_file(char *path) {
    struct stat st;
    if (stat(path, &st) == -1) {
        fprintf(stderr, "No se pudo aplicar stat sobre el archivo %s\n", path);
        return -1;
    }

    return S_ISREG(st.st_mode);
} */

/**
 * Función que recorre recursivamente 
 * 
 * Parámetros:
 *      path: ruta del directorio
 *      name_to_find: nombre del archivo 
 * Retorno:
 *
 */
int traverseDir(char *path, char *name_to_find) {
    DIR* dir;
    struct dirent* ent;

    /* Si no existe el directorio */
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

        if (!dots) {
            int is_dir = is_dir_file(new_path);
            if (is_dir == -1) {
                return -1;
            }

            /* Si es un directorio, sigue recorriendo */
            if (is_dir) {
                int status = traverseDir(new_path, name_to_find);
                if (status == -1) {
                    return -1;
                }
            } else {
                /* Si es un archivo, REALIZAR ACCIÓN */
                if (strstr(e_name, name_to_find)) {
                    printf("%s\n", new_path);
                }
            }
        }
        free(new_path);
    }

    closedir(dir);
    return 0;
}


void find(char *path, char *name) {
    if (strstr(path, name)) {
        printf("%s\n", path);
    }
}

int ifind(char *path, char *name) {
    char *path_to_compare = path;
    char *name_to_compare = name;
    char *newName, *newPath;
    int i;

    /* Convertimos el nombre a buscar en minuscula */
    newName = malloc(strlen(name) + 1);
    if (newName == NULL) {
        fprintf(stderr, "Error al asignar memoria\n");
        return -1;
    }
    strcpy(newName, name);
    for (i = 0; i < strlen(newName); i++) {
        newName[i] = toupper(newName[i]);
    }
    name_to_compare = newName;

    /* Convertimos el nombre del archivo en minuscula */
    newPath = malloc(strlen(path) + 1);
    if (newPath == NULL) {
        fprintf(stderr, "Error al asignar memoria\n");
        return -1;
    }
    strcpy(newPath, path);
    for (i = 0; i < strlen(newPath); i++) {
        newPath[i] = toupper(newPath[i]);
    }
    path_to_compare = newPath;

    /* Llamamos find con el nuevo nombre de archivo y nombre */
    find(path_to_compare, name_to_compare);

    free(newName);
    free(newPath);

    return 0;
}

int cfind(char *path, char *string, char *string2) {
    char *buffer;
    int fd;

    /* Si el nombre del archivo no tiene la cadena string */
    if (strstr(path, string) == NULL) {
        return 0;
    }
    
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
        if (strstr(buffer, string2)) {
            printf("%s\n", path);
        }
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
int wc(char *path, int *lines, int *chars) {
    FILE *ptr = fopen(path, "r");
    char c;

    if (!ptr) {
        return -1;
    }

    while (fread(&c, 1, 1, ptr) > 0) {
        if (c == '\n') {
            (*lines)++;
        }
        (*chars)++;
    }

    fclose(ptr);

    return 0;
}

int wc2(char *path, int *lines, int *chars) {
    char c;
    int fd = open(path, O_RDONLY);

    /* Abre el archivo */
    if (fd == -1) {
        fprintf(stderr, "Error al abrir el archivo %s\n", path);
        return -1;
    }

    /* Por cada caracter, cuenta cuantos saltos de líneas y caracteres hay */
    while (read(fd, &c, 1) > 0) {
        if (c == '\n') {
            (*lines)++;
        }
        (*chars)++;
    }

    close(fd);
    return 0;
}

/**
 * Función que invierte el contenido de un archivo
 * 
 */
int codif(char *path) {
    char inicio, fin;
    int filesize;
    int len1, len2, n;
    int fd1 = open(path, O_RDWR);
    int fd2 = open(path, O_RDWR);

    /* Abre el archivo */
    if (fd1 == -1) return -1;

    /* Invierte el contenido del archivo */
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
} 

/**
 * Función que invierte el contenido de un archivo
 * 
 * Parámetros:
 *      path: ruta del archivo a invertir el contenido
 * Retorno:
 *      0 si todo fue correcto, -1 si hubo un error
 */
int codif2(char *path) {
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
int roll(char *path, int n) {
    int fd = open(path, O_RDONLY);
    int lines = 0;
    int chars = 0;
    char c;

    close(fd);
    return 0;
}