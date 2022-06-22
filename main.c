#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>

int is_dir_file(char *path);
int is_reg_file(char *path);
int find(char *path, char *name, int ifind, char *cfind);
void find_type(char *path, char *name, int ifind, char *cfind);

int main(int argc, char **argv) {
    char* rootDir;

    if (argc != 2) {
        printf("Uso: ./myutil <directorioRaiz>\n");
        return 1;
    }

    rootDir = argv[1];
    printf("Buscando en %s\n", rootDir);

    find(rootDir, "txt", 1, NULL);
    return 0;
}

/**
 * @brief 
 * 
 * @param path 
 * @param name 
 * @param ifind 0 si se trata de ifind, en cambio 1
 * @param cfind NULL si no se trata de cfind, en cambio la cadena a buscar
 * @return int 
 */
int find(char *path, char *name, int ifind, char *cfind) {
    DIR* dir;
    struct dirent* ent;

    if (!is_reg_file(path) && (dir = opendir(path)) == NULL) {
        fprintf(stderr, "Error al abrir el directorio %s\n", path);
        return 1;
    } else if (is_reg_file(path)) {
        return 0;
    }
    
    while ((ent = readdir(dir))) {
        int e_type = ent->d_type;
        char* e_name = ent->d_name;
        int dots = strcmp(e_name, ".") == 0 || strcmp(e_name, "..") == 0;

        /* Concatena la direccion */
        char* newPath = malloc(strlen(path) + strlen(e_name) + 2);
        strcpy(newPath, path);
        strcat(newPath, "/");
        strcat(newPath, e_name);

        if (!dots) {
            if (is_dir_file(path)) {
                find(newPath, name, ifind, cfind);
            } 
            
            if (is_reg_file(newPath)) {
                find_type(newPath, name, ifind, cfind);
            }
        }
        free(newPath);
    }
    closedir(dir);
    return 0;
}

void find_type(char *path, char *name, int ifind, char *cfind) {
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

/**
 * @brief 
 * 
 * @param path 
 * @return int 
 */
int is_dir_file(char *path) {
    struct stat st;
    stat(path, &st);
    return S_ISDIR(st.st_mode);
}

int is_reg_file(char *path) {
    struct stat st;
    stat(path, &st);
    return S_ISREG(st.st_mode);
}