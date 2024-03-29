/**
 * repla.c
 * 
 * Programa que toma una lista de pares de cadenas de caracteres 
 * <cad1, cad2> de un archivo y reemplaza, en un conjunto de 
 * archivos de texto, todas las ocurrencias de cad1 por cad2.
 * 
 * Autor: Ka Fung (18-10492)
 * Fecha: 16/06/2020 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include "lista.h"
#include "misc.h"

int repla_aux(char *archivo, void *args);
char *temp_name(int length);
struct Nodo *extraer_palabras(char *archivo);

/**
 * Funcion que reemplaza una cadena de caracteres por otra en los 
 * archivos de un directorio raíz dado.
 * 
 * Parámetros:
 *      dir_raiz: ruta del directorio raíz
 *      file: lista de pares de cadenas de caracteres
 */
void repla(char *dir_raiz, char *file) {
    struct Nodo *lista = extraer_palabras(file);
    if (!lista) {
        fprintf(stderr, "Error al extraer las palabras del archivo %s\n", file);
        return;
    }

    if (traverse_dir(dir_raiz, repla_aux, lista, 0) == -1) 
        fprintf(stderr, "Error al ejecutar repla.\n");
    
    liberar_lista(lista);
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
int repla_aux(char *archivo, void *args) {
    struct Nodo* cabeza = (struct Nodo*) args;
    char ch, *temp_archivo = temp_name(10);   
    int ptr, write_ptr, len;

    struct stat st;
    if (stat(archivo, &st) == -1) return -1;

    /* Abrir el archivo a leer */
    ptr = open(archivo, O_RDONLY);
    if (ptr == -1) return -1;
    
    /* Abrir el archivo temporal a escribir */
    write_ptr = open(temp_archivo, O_WRONLY | O_CREAT, st.st_mode);
    if (write_ptr == -1) return -1;

    /* Revisamos por cada coincidencia de char del archivo a reemplazar */
    while ((len = read(ptr, &ch, 1)) > 0 && ch != EOF) {  

        /* Revisamos por cada palabra de la lista */
        struct Nodo* actual = cabeza;
        long int pos = lseek(ptr, 0, SEEK_CUR) - 1;
        while (actual != NULL) {

            /* Itera mientras coincidan la palabra de la lista y el texto*/
            int i = 0;
            while (ch == actual->dato->x[i]) {
                if ((len = read(ptr, &ch, 1)) == 0) break;
                i++;
            }

            /* Si coincide toda la palabra, se imprime */
            if (i == actual->len) {
                write(write_ptr, actual->dato->y, strlen(actual->dato->y));
                lseek(ptr, pos + i, SEEK_SET);
                break;
            }

            /* En cambio, se revisa con la siguiente palabra de la lista */
            actual = actual->next;
            lseek(ptr, pos, SEEK_SET);
            if ((len = read(ptr, &ch, 1)) == 0) break;
        }

        /* Si no coincide ninguna palabra de la lista, se imprime el char */
        if (!actual)
            write(write_ptr, &ch, 1);
    }

    close(ptr);
    close(write_ptr);

    /* Renombra el archivo temporal, si no se puede, se elimina */
    if (rename(temp_archivo, archivo) != 0) {
        free(temp_archivo);
        remove(temp_archivo);
        return -1;
    }

    /* Liberar nombre del archivo temporal */
    /* chmod(archivo, st.st_mode); */
    free(temp_archivo);

    return 0;
}

/**
 * Genera un string aleatorio de longitud dada.
 * Parametros:
 *      - length: longitud del string 
 * Retorno:
 *      - string aleatorio
 */
char *temp_name(int length) {
    int i;
    
    char *string = malloc(sizeof(char) * (length + 11));
    if (!string) return NULL;

    for (i = 0; i < length; i++) {
        srand(clock());
        string[i] = 'a' + (rand() % 26);
    }
    string[0] = '.';
    string[i] = '\0';
    strcat(string, "-repla.tmp");
    return string;
}

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
    if (!ptr) return NULL;

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