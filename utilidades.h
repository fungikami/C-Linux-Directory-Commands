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
        struct Nodo* palabras;
        int n;
        int to_print;
    };

    int is_dir_file(char *path);
    int find(struct Args *args);
    int ifind(struct Args *args);
    int cfind(struct Args *args);
    int codif(struct Args *args);
    int count_lines_chars(char *path, int *lines, int *chars);
    int wc(char *path, int *nlines, int *nchars);
    int traverseDir(char* path, int (*fun) (struct Args* argum), struct Args* argum, int action_to_dir);
#endif