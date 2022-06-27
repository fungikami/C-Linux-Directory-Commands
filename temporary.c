
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

/**
 * Función que invierte el contenido de un archivo
 * 
 * Parámetros:
 *      path: ruta del archivo a invertir el contenido
 * Retorno:
 *      0 si todo fue correcto, -1 si hubo un error
 */
int codif(struct Args *args) {
    char *path = args->path;
    FILE *fp = fopen(path, "r+");
    char inicio, fin;
    long m, n, filesize;

    /* Abre el archivo */
    if (!fp) return -1;

    /* Invierte el contenido del archivo */
    fseek(fp, -1,  SEEK_END);
    filesize = ftell(fp);

    m = 0;
    n = filesize / 2;
    while (n) {
        fseek(fp, m++, SEEK_SET);
        inicio = fgetc(fp);

        fseek(fp, -m, SEEK_END);
        fin = fgetc(fp);

        fseek(fp, -m, SEEK_END);
        fprintf(fp, "%c", inicio);

        fseek(fp, m-1, SEEK_SET);
        fprintf(fp, "%c", fin);

        n--;
    }

    fclose(fp);
    return 0;
}

int cfind(char *path, struct Args *args) {
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
            free(line);
            fclose(stream);
            return 1;
        }
    }

    free(line);
    fclose(stream);
    return 0;
}

/* MALOOO */
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