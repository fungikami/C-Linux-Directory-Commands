/**
 * main.c
 *
 * Implementación del interpretador de comandos simple, en que se ofrecen un
 * conjunto de comandos internos al invocarse con ./myutil <directorioRaiz>.
 *
 * Comandos:
 *      find [<cadena>]: imprime los nombres de archivos que tienen en su nombre
 *            la cadena <cadena>, case sensitive.

 *      ifind [<cadena>]: igual que find, pero no case sensitive.

 *      cfind <cadena1> <cadena2>: igual que find, pero adicionalmente verifica
 *            contenido del archivo tenga la cadena <cadena2>.

 *      repla <file>: dado un archivo de pares de cadenas, sustituye en los 
 *            archivos regulares las ocurrencias de dichas cadenas de caracteres.

 *      wc: imprime por directorio el total de líneas y caracteres de los archivos 
 *              regulares.

 *      codif: invierte el contenido de los archivos regulares.
 
 *      roll [<n>]: rota en n caracteres el contenido de cada archivo regular.
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
    char *dir_raiz;
    struct stat st;

    /* Verifica que se reciba el directorio raiz */
    if (argc != 2) {
        printf("Uso: ./myutil <directorioRaiz>\n");
        return 1;
    }

    /* Verifica que el directorio raiz pueda ser abierto */
    dir_raiz = argv[1];
    if (stat(dir_raiz, &st) == -1) {
        printf("Error: El directorio raiz %s no puede ser abierto\n", dir_raiz);
        return 1;
    }

    /* Pide los comandos a ejecutar */
    while (1) {
        char *cadena, *comando, separator = ' ';
        printf("myutil> ");
        comando = get_line();


        /* Separa los argumentos de la línea de comando */
        cadena = strchr(comando, separator);    
        if (strcmp(comando, "") == 0) {
            free(comando);
            continue;
        }
        if (cadena) {
            *cadena = '\0';
            cadena++;
        }

        /* Ejecuta la función correspondiente según el comando invocado */
        if (!strcmp(comando, "find")) {
            remove_quotes(cadena);
            find(dir_raiz, cadena);
        } else if (!strcmp(comando, "ifind")) {
            remove_quotes(cadena);
            ifind(dir_raiz, cadena);
        } else if (!strcmp(comando, "cfind")) {
            char *cadena2;
            if (!cadena || !(cadena2 = strchr(cadena, separator))) {
                fprintf(stderr, "Error: El comando debe ser de la forma: cfind <cadena1> <cadena2>\n");
                free(comando);
                continue;
            }
            *cadena2 = '\0';
            cadena2++;
            cfind(dir_raiz, cadena, cadena2);
        } else if (!strcmp(comando, "repla")) {
            if (!cadena) {
                fprintf(stderr, "Error: El comando debe ser de la forma: repla <file>\n");
                free(comando);
                continue;
            }
            repla(dir_raiz, cadena);
        } else if (!strcmp(comando, "wc")) {
            if (cadena) {
                fprintf(stderr, "Error: El comando debe ser de la forma: wc\n");
                free(comando);
                continue;
            }
            wc(dir_raiz);
        } else if (!strcmp(comando, "codif")) {
            if (cadena) {
                fprintf(stderr, "Error: El comando debe ser de la forma: codif\n");
                free(comando);
                continue;
            }
            codif(dir_raiz);
        } else if (!strcmp(comando, "roll")) {
            int n = 0;
            char *number = cadena;
            if (number) {
                if (!is_integer(number)) {
                    fprintf(stderr, "Error: El comando debe ser de la forma: roll [<n>] con n entero.\n");
                    free(comando);
                    continue;
                }
                n = atoi(number);
            }
            roll(dir_raiz, n);
        } else if (!strcmp(comando, "exit")) {
            free(comando);
            break;
        } else {
            fprintf(stderr, "Error: Comando no reconocido\n");
        }

        free(comando);
    }
    
    return 0;
}