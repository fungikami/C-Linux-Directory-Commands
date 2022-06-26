/**
 * utilidades.h
 * 
 * Autor: Ka Fung (1810492)
 * Fecha: 08/07/2020 
 */

#ifndef __UTILIDADES_H__
    #define __UTILIDADES_H__

    struct Args {
        char* path;
        char* cadena1;
        char* cadena2;
        struct Nodo* lista;
        int n;
    };

    int is_dir_file(char *path);
    int is_reg_file(char *path);
    int codif(char *path, struct Args *args);
    int roll(char *path, int n);
    int traverseDir(char* path, int (*fun) (char *path, struct Args* argum), struct Args* argum, int action_to_dir);
#endif