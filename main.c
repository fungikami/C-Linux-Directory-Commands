#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

int is_dir_file(char *path);
int is_reg_file(char *path);
int find(char *path, char *name, int ifind, char *cfind) ;

int main(int argc, char **argv) {
    char* rootDir;
    char
    if (argc != 2) {
        printf("Uso: ./myutil <directorioRaiz>\n");
        return 1;
    }

    rootDir = argv[1];
    printf("Hello, world!\n");
    printf("Buscando en %s\n", rootDir);
    find(rootDir, "algo", 0, NULL);
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

    if ((dir = opendir(path)) == NULL) {
        /* fprintf(stderr, "Error al abrir el directorio %s\n", path); */
        return 1;
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
            char *ext = strrchr(e_name, '.') ;

            if (is_dir_file(path)) {
                find(newPath, name, ifind, cfind);
            } 
            
            if (strcmp(e_name, name) == 0 || 
                (ext && strcmp(ext + 1, name) == 0)) {
                printf("%s\n", newPath);
            }
        }
        free(newPath);
    }
    closedir(dir);
    return 0;
}

int find_type(char *path, char *name, int ifind, char *cfind) {
    
    return 0;
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