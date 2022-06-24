/**
 * utilidades.h
 * 
 * Autor: Ka Fung (1810492)
 * Fecha: 08/07/2020 
 */

#ifndef __UTILIDADES_H__
    #define __UTILIDADES_H__

    int is_dir_file(char *path);
    int is_reg_file(char *path);
    int traverseDir(char *path, char *name);
    void find(char *path, char *name);
    int ifind(char *path, char *name);
    int codif(char *path);
    int wc(char *path, int *lines, int *chars);
#endif