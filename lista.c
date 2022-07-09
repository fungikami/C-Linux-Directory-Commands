/**
 * Lista.c
 * Lista Doblemente Enlazada de Pares de palabras
 * Autor: Ka Fung (18-10492)
 * Fecha: 16/06/2020 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"
#include "par.h"

/**
 * Función que crea una lista doblemente enlazada de pares de palabras.
 * Retorno:
 *    - Cabeza de la lista doblemente enlazada de pares de palabras.
 */
struct Nodo* crear_lista() {
    struct Nodo *cabeza = (struct Nodo*)malloc(sizeof(struct Nodo));
    if (!cabeza) return NULL;

    cabeza->dato = NULL;
    cabeza->prev = NULL;
    cabeza->next = NULL;

    return cabeza;
}

/**
 * Inserta un nuevo nodo en una lista ordenada descendentemente.
 * Se toma como comparación la longitud de la primera palabra del par.
 * Parametros:
 *      - cabeza: cabeza de la lista
 *      - palabras: palabras a insertar
 * Retorno:
 *      - 0 si se pudo insertar, -1 si no se pudo insertar
 */ 
int insertar_ordenado_lista(struct Nodo** cabeza, Par* palabras) {
    int len_palabra = strlen(palabras->x);
    struct Nodo* nuevo = (struct Nodo*)malloc(sizeof(struct Nodo));
    if (!nuevo) return -1;

    /* Crea nuevo nodo */
    nuevo->dato = palabras;
    nuevo->len = len_palabra;

    if (!(*cabeza)->dato) {
        /* Si la lista esta vacia, simplemente se agrega */
        *cabeza = nuevo;
        nuevo->next = NULL;
        nuevo->prev = NULL;
    } else if (len_palabra > (*cabeza)->len) {
        /* Si la palabra es mayor que la primera, se agrega al inicio */
        nuevo->next = *cabeza;
        nuevo->prev = NULL;
        (*cabeza)->prev = nuevo;
        *cabeza = nuevo;
    } else {
        /* En cambio, se busca en donde insertar */
        struct Nodo* actual = *cabeza;
        while (actual->next != NULL && len_palabra < actual->next->len) {
            actual = actual->next;
        }
        
        /* Actualiza los punteros */
        nuevo->next = actual->next;
        if (actual->next) {
            actual->next->prev = nuevo;
        }
        actual->next = nuevo;
        nuevo->prev = actual;
    }
    return 0;
}

/**
 * Imprime la lista doblemente enlazada
 * Parámetros:
 *     - nodo: puntero a la cabeza de la lista
 */
void print_lista(struct Nodo* nodo) {
    printf("[");
    while (nodo) {
        print_par(nodo->dato);
        if (nodo->next) {
            printf(", ");
        }
        nodo = nodo->next;
    }
    printf("]\n");
}

/**
 * Libera la memoria asignada de la lista doblemente enlazada
 * Parámetros:
 *    - nodo: puntero a la cabeza de la lista
 */ 
void liberar_lista(struct Nodo* nodo) {
    while (nodo) {
        struct Nodo* temp = nodo;
        nodo = nodo->next;
        liberar_par(temp->dato);
        free(temp);
    }
}