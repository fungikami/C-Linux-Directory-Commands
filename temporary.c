
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>

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

void find(char *path, char *name, int ifind, char *cfind) {
    char* path_to_compare = path;
    char* name_to_compare = name;

    /* Si se trata de un ifind, convertimos todo a uppercase */
    if (ifind) {
        int i;
        char *newName, *newPath;

        newName = malloc(strlen(name) + 1);
        strcpy(newName, name);
        for (i = 0; i < strlen(newName); i++) {
            newName[i] = toupper(newName[i]);
        }
        name_to_compare = newName;

        newPath = malloc(strlen(path) + 1);
        strcpy(newPath, path);
        for (i = 0; i < strlen(newPath); i++) {
            newPath[i] = toupper(newPath[i]);
        }
        path_to_compare = newPath;
    }

    /* Si el nombre de path tiene la cadena name */
    if (strstr(path_to_compare, name_to_compare)) {
        /* Si se trata de un cfind, verificamos que dentro del archivo tenga la cadena cfind */
        if (cfind) {
            int fd = open(path, O_RDONLY);

            printf("todo");
        } else {
        /* Si no se trata de un cfind, imprimimos el path */
            printf("%s\n", path);
        }
    }
}

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
    close(fd);
    return 0;
}

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

        /* Recorre recursivamente por cada directorio */
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