#include <stdio.h>
#include <stdlib.h>
#define SIZE 100

int main(int argc, char **argv) {
    FILE *fi1, *fi2;
    char buf[SIZE];
    int lenr, lenw;

    /* Verifico que efectivamente tengo los dos nombres. */
    if (argc != 3) {
        fprintf(stderr, "Error. Sintaxis: %s: nom1 nom2\n", argv[0]);
        exit(-1);
    }

    /* Copia */
    fi1 = fopen(argv[1], "r");
    if (fi1 == NULL) exit(-1);
    fi2 = fopen(argv[2], "w");
    if (fi2 == NULL) exit(-1);

    while ((lenr = fread(buf, sizeof(char), SIZE, fi1)) > 0) {
        lenw = 0;
        do {
           lenw += fwrite(buf + lenw, sizeof(char), lenr-lenw, fi2);
        } while (lenw < lenr);
    }
    fclose(fi1);
    fclose(fi2);

    return 0;
}