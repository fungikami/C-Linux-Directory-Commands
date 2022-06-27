/**
 * repla.c
 * 
 * Programa que toma una lista de pares de cadenas de caracteres 
 * <cad1, cad2> de un archivo y reemplaza, en un conjunto de 
 * archivos de texto, todas las ocurrencias de cad1 por cad2.
 * 
 * Autor: Ka Fung (1810492)
 * Fecha: 16/06/2020 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include "lista.h"
#include "repla.h"

/** 
 * Dado un archivo con una lista de palabras a sustituir, extrae 
 * las palabras y las guarda en una lista doblemente enlazada dada.
 * Parámetros:
 * 		- archivo: nombre del archivo
 *      - cabeza: puntero a la lista
 */
struct Nodo* extraer_palabras(char* archivo) {
    char temp1[60], temp2[60];
    struct Nodo* lista_cabeza = crear_lista();

    FILE* ptr = fopen(archivo, "r");
    if (!ptr) {
        return NULL;
    }

    /* Extrae las palabras y las separa según el char ':' */
    while(fscanf(ptr, "%[^:]:%[^\n]\n", temp1, temp2) != EOF) {
        Par* palabras;
        int i;
        char* word1 = (char *)malloc(sizeof(char) * strlen(temp1) + 1);
        char* word2 = (char *)malloc(sizeof(char) * strlen(temp2) + 1);
        if (!word1 || !word2) {
            fclose(ptr);
            return NULL;
        }

        /* Copia las palabras y guarda en la lista doblemente enlazada */
        strcpy(word1, temp1);
        strcpy(word2, temp2);

        /* Crea par de palabras */
        palabras = crear_par(word1, word2);
        if (!palabras) {
            free(word1);
            free(word2);
            fclose(ptr);
            return NULL;
        }

        /* Inserta par de palabras en la lista */
        i = insertar_ordenado_lista(&lista_cabeza, palabras);
        if (i < 0) {
            liberar_lista(lista_cabeza);
            fclose(ptr);
            return NULL;
        }
    }

    fclose(ptr);
    return lista_cabeza;
}

/** 
 * Dado un archivo de texto y una lista de palabras a reemplazar,
 * reemplaza todas las ocurrencias de las palabras en el archivo.
 * Parametros:
 *      - archivo: archivo a reemplazar
 *      - cabeza: cabeza de la lista doblemente enlazada
 * Retorno:
 *      - 0 si se pudo reemplazar, -1 si no se pudo reemplazar
 */
int repla(char* archivo, struct Args *args) {
    struct Nodo* cabeza = args->lista;
    char ch;
    char* temp_archivo = random_string(10);   
    FILE *ptr, *write_ptr;

    struct stat st;
    if (stat(archivo, &st) == -1) return -1;

    /* Abrir el archivo a leer */
    ptr = fopen(archivo, "r");
    if (!ptr) return -1;
    
    /* Abrir el archivo temporal a escribir */
    write_ptr = fopen(temp_archivo, "w");
    if (!write_ptr) {
        free(temp_archivo);
        fclose(ptr);
        return -1;
    }

    /* Revisamos por cada coincidencia de char del archivo a reemplazar */
    ch = fgetc(ptr);
    while (ch != EOF) {   
        /* Revisamos por cada palabra de la lista */
        struct Nodo* actual = cabeza;
        while (actual != NULL) {
            /* Itera mientras coincidan la palabra de la lista y el texto*/
            int i = 0;
            
            while (ch == actual->dato->x[i]) {
                ch = fgetc(ptr);
                i++;
            }

            /* Si coincide toda la palabra, se imprime */
            if (i == actual->len) {
                fprintf(write_ptr, "%s", actual->dato->y);
                break;
            }

            /* En cambio, se revisa con la siguiente palabra de la lista*/
            actual = actual->next;
            fseek(ptr, -i-1, SEEK_CUR);
            ch = fgetc(ptr);
        }

        /* Si no coincide ninguna palabra de la lista, se imprime el char */
        if (!actual) {
            fprintf(write_ptr, "%c", ch);
            ch = fgetc(ptr);
        } 
    }

    fclose(ptr);
    fclose(write_ptr);

    /* Renombra el archivo temporal */
    if (rename(temp_archivo, archivo) != 0) {
        free(temp_archivo);
        remove(temp_archivo);
        return -1;
    }

    free(temp_archivo);
    chmod(archivo, st.st_mode);
    return 0;
}

/**
 * Genera un string aleatorio de longitud dada.
 * Parametros:
 *      - length: longitud del string 
 */
char *random_string(int length) {
    int i;
    
    char *string = malloc(length + 11);
    if (!string) {
        return NULL;
    }

    for (i = 0; i < length; i++) {
        srand(clock());
        string[i] = 'a' + (rand() % 26);
    }
    string[i] = '\0';
    strcat(string, "-repla.tmp");
    return string;
}