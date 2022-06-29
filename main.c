/**
 * main.c
 * 
 * Autor: Ka Fung (18-10492)
 * Fecha: 08/07/2020 
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "utilidades.h"
#include "finds.h"
#include "repla.h"
#include "wc.h"
#include "codif.h"
#include "roll.h"

int main(int argc, char **argv) {
    char *dirRaiz;
    char *str;
    struct stat st;

    /* Verifica que se reciba el directorio raiz */
    if (argc != 2) {
        printf("Uso: ./myutil <directorioRaiz>\n");
        return 1;
    }

    /* Verifica que el directorio raiz pueda ser abierto */
    dirRaiz = argv[1];
    if (stat(dirRaiz, &st) == -1) {
        printf("Error: El directorio raiz %s no puede ser abierto\n", dirRaiz);
        return 1;
    }

    /* Pide los comandos a ejecutar */
    while (1) {
        char * token;
        printf("myutil> ");
        str = get_line();
        if (!str) fprintf(stderr, "Error al leer la línea\n");

        /* Separa los argumentos de la línea de comando */
        token = strtok(str, " ");
        if (!token) {
            free(str);
            continue;
        }
        
        /* Ejecuta la función correspondiente según el comando invocado */
        if (!strcmp(token, "find")) {
            char *cadena = strtok(NULL, " ");
            find(dirRaiz, cadena);
        } else if (!strcmp(token, "ifind")) {
            char *cadena = strtok(NULL, " ");
            ifind(dirRaiz, cadena);
        } else if (!strcmp(token, "cfind")) {
            char *cadena1 = strtok(NULL, " ");
            char *cadena2 = strtok(NULL, " ");
            char *cadena3 = strtok(NULL, " ");

            if (!cadena1 || !cadena2 || cadena3) {
                fprintf(stderr, "Error: El comando debe ser de la forma: cfind <cadena1> <cadena2>\n");
                free(str);
                continue;
            }

            cfind(dirRaiz, cadena1, cadena2);
        } else if (!strcmp(token, "repla")) {
            char *file = strtok(NULL, " ");
            char *wrong_file = strtok(NULL, " ");

            if (!file || wrong_file) {
                fprintf(stderr, "Error: El comando debe ser de la forma: repla <file>\n");
                free(str);
                continue;
            }

            repla(dirRaiz, file);
        } else if (!strcmp(token, "wc")) {
            if (strtok(NULL, " ")) {
                fprintf(stderr, "Error: El comando debe ser de la forma: wc\n");
                free(str);
                continue;
            }

            wc(dirRaiz);
        } else if (!strcmp(token, "codif")) {
            if (strtok(NULL, " ")) {
                fprintf(stderr, "Error: El comando debe ser de la forma: codif\n");
                free(str);
                continue;
            }

            codif(dirRaiz);
        } else if (!strcmp(token, "roll")) {
            int n = 0;
            char *number = strtok(NULL, " ");
            if (number) n = atoi(number);

            roll(dirRaiz, n);
        } else if (!strcmp(token, "exit")) {
            free(str);
            break;
        } else {
            fprintf(stderr, "Error: Comando no reconocido\n");
        }

        free(str);
    }
    
    return 0;
}