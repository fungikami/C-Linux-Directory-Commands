#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"
#include "repla.h"

int main(int argc, char **argv) {
    struct Nodo* lista_cabeza;
    int i;

    /* Revisar que recibe correctamente los argumentos */
    if (argc < 3) {
        printf("Uso: sustituir <palabras.txt> <Archivo1.txt> <Archivo2.txt>... \n");
        return 1;
    }

    /* Extraer palabras y almacenarlas en la lista doblemente enlazada */
    lista_cabeza = extraer_palabras(argv[1]);
    if (!lista_cabeza) {
        fprintf(stderr, "Error al extraer palabras\n");
        return -1;
    }

    /* Por cada archivo, reemplazar las palabras */
    for (i = 2; i < argc; i++) {
        int j = remplazar_palabras(argv[i], lista_cabeza);
        if (j < 0) {
            fprintf(stderr, "Error al reemplazar palabras\n");
            return -1;
        }
        
        if (i != argc - 1) {
            printf("\n--\n");
        } 
    }

    /* Liberar memoria ocupada por la lista */
    liberar_lista(lista_cabeza);

    return 0;
}

int repla(char* archivo) {
    
}