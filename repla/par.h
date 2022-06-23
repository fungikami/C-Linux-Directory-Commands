/**
 * Par.h
 * Par de palabras
 * Autor: Ka Fung (1810492)
 * Fecha: 16/06/2020 
 */

#ifndef __PAR_H__
    #define __PAR_H__
     
    /**
     * Par de palabras
     * Atributos:
     *     - x: palabra a remplazar
     *     - y: palabra por remplazar
     */
    typedef struct {
        char *x;
        char *y;
    } Par;

    Par* crear_par(char* x, char* y);
    void print_par(Par* par);
    void liberar_par(Par* par);
#endif