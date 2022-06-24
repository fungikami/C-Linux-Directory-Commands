
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>

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
