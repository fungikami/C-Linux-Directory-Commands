#include <stdio.h>
#include <dirent.h>

int find(char *path, char *name);

int main(int argc, char **argv) {
    char* rootDir;

    if (argc != 2) {
        printf("Uso: ./myutil <directorioRaiz>\n");
        return 1;
    }

    rootDir = argv[1];
    printf("Hello, world!\n");

    find(rootDir, "*.c");
    return 0;
}

/**
 * @brief 
 * 
 * @param path 
 * @param name 
 * @return int 
 */
int find(char *path, char *name) {
    DIR* dir;
    struct dirent* ent;

    if ((dir = opendir(path)) == NULL) {
        fprintf(stderr, "Error al abrir el directorio %s\n", path);
        return 1;
    }
    
    printf("%s\n", path);
    printf("%s\n", name);
    return 0;
}