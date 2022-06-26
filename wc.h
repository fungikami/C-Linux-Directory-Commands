/**
 * wc.h
 * 
 * Autor: Ka Fung (1810492)
 * Fecha: 08/07/2020 
 */

#ifndef __WC_H__
    #define __WC_H__

    int count_lines_chars(char *path, int *lines, int *chars);
    int wc(char *path, int *nlines, int *nchars);
#endif