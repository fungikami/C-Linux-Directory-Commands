/**
 * utilidades.h
 * 
 * Autor: Ka Fung (18-10492)
 * Fecha: 08/07/2020 
 */

#ifndef __UTILIDADES_H__
    #define __UTILIDADES_H__

    void remove_quotes(char *str);
    char *get_line();
    int is_integer(char *str);
    int is_dir_file(char *path);
    int is_reg_file(char *path);
    int traverseDir(char *path, int (*fun) (char *, void *), void *argum, int action_to_dir);
#endif