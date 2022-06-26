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
    int find(char *path, struct Args *args);
    int ifind(char *path, struct Args *args);
    int cfind(char *path, struct Args *args);
    int cfind2(char *path, struct Args *args);
    int codif(char *path, struct Args *args);
    int count_lines_chars(char *path, int *lines, int *chars);
    int wc(char *path, int *nlines, int *nchars);
    int roll(char *path, int n);
    int traverseDir(char* path, int (*fun) (char *path, struct Args* argum), struct Args* argum, int action_to_dir);
#endif