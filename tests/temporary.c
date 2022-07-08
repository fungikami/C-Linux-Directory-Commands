
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>

/**
 * Funcion que determina si un archivo es regular
 * 
 * Parámetros:
 *      path: ruta del archivo
 */
int is_reg_file(char *path) {
    struct stat st;
    if (stat(path, &st) == -1) {
        fprintf(stderr, "No se pudo aplicar stat sobre el archivo %s\n", path);
        return -1;
    }

    return S_ISREG(st.st_mode);
} 

void find(char *path, char *name, int ifind, char *cfind) {
    char* path_to_compare = path;
    char* name_to_compare = name;

    /* Si se trata de un ifind, convertimos todo a uppercase */
    if (ifind) {
        int i;
        char *newName, *newPath;

        newName = malloc(strlen(name) + 1);
        strcpy(newName, name);
        for (i = 0; i < strlen(newName); i++) {
            newName[i] = toupper(newName[i]);
        }
        name_to_compare = newName;

        newPath = malloc(strlen(path) + 1);
        strcpy(newPath, path);
        for (i = 0; i < strlen(newPath); i++) {
            newPath[i] = toupper(newPath[i]);
        }
        path_to_compare = newPath;
    }

    /* Si el nombre de path tiene la cadena name */
    if (strstr(path_to_compare, name_to_compare)) {
        /* Si se trata de un cfind, verificamos que dentro del archivo tenga la cadena cfind */
        if (cfind) {
            int fd = open(path, O_RDONLY);

            printf("todo");
        } else {
        /* Si no se trata de un cfind, imprimimos el path */
            printf("%s\n", path);
        }
    }
}

int wc(char *path, int *lines, int *chars) {
    FILE *ptr = fopen(path, "r");
    char c;

    if (!ptr) {
        return -1;
    }

    while (fread(&c, 1, 1, ptr) > 0) {
        if (c == '\n') {
            (*lines)++;
        }
        (*chars)++;
    }

    fclose(ptr);

    return 0;
}

int count_lines_chars(char *path, int *lines, int *chars) {
    char c, buf[BUFSIZ];
    int len, i;
    int fd = open(path, O_RDONLY);

    /* Abre el archivo */
    if (fd == -1) {
        fprintf(stderr, "Error al abrir el archivo %s\n", path);
        return -1;
    }

    /* Por cada caracter, cuenta cuantos saltos de líneas y caracteres hay */
    while ((len = read(fd, buf, BUFSIZ)) > 0) {
        for (i = 0; i < len; i++) {
            c = buf[i];
            if (c == '\n') {
                (*lines)++;
            }
            (*chars)++;
        }
    }
    close(fd);
    return 0;
}

/**
 * Función que recorre recursivamente 
 * 
 * Parámetros:
 *      path: ruta del directorio
 *      name_to_find: nombre del archivo 
 * Retorno:
 *
 */
int traverse_dir(char *path, char *name_to_find) {
    DIR* dir;
    struct dirent* ent;

    /* Si no existe el directorio */
    if (!(dir = opendir(path))) {
        fprintf(stderr, "Error al abrir el directorio %s\n", path);
        return -1;
    } 
    
    while ((ent = readdir(dir))) {
        char* e_name = ent->d_name;
        int dots = strcmp(e_name, ".") == 0 || strcmp(e_name, "..") == 0;

        /* Concatena la nueva direccion */
        char* new_path = malloc(strlen(path) + strlen(e_name) + 2);
        strcpy(new_path, path);
        strcat(new_path, "/");
        strcat(new_path, e_name);

        /* Recorre recursivamente por cada directorio */
        if (!dots) {
            int is_dir = is_dir_file(new_path);
            if (is_dir == -1) {
                return -1;
            }

            /* Si es un directorio, sigue recorriendo */
            if (is_dir) {
                int status = traverse_dir(new_path, name_to_find);
                if (status == -1) {
                    return -1;
                }
            } else {
                /* Si es un archivo, REALIZAR ACCIÓN */
                if (strstr(e_name, name_to_find)) {
                    printf("%s\n", new_path);
                }
            }
        }
        free(new_path);
    }

    closedir(dir);
    return 0;
}

/**
 * Función que invierte el contenido de un archivo
 * 
 * Parámetros:
 *      path: ruta del archivo a invertir el contenido
 * Retorno:
 *      0 si todo fue correcto, -1 si hubo un error
 */
int codif(struct Args *args) {
    char *path = args->path;
    FILE *fp = fopen(path, "r+");
    char inicio, fin;
    long m, n, filesize;

    /* Abre el archivo */
    if (!fp) return -1;

    /* Invierte el contenido del archivo */
    fseek(fp, -1,  SEEK_END);
    filesize = ftell(fp);

    m = 0;
    n = filesize / 2;
    while (n) {
        fseek(fp, m++, SEEK_SET);
        inicio = fgetc(fp);

        fseek(fp, -m, SEEK_END);
        fin = fgetc(fp);

        fseek(fp, -m, SEEK_END);
        fprintf(fp, "%c", inicio);

        fseek(fp, m-1, SEEK_SET);
        fprintf(fp, "%c", fin);

        n--;
    }

    fclose(fp);
    return 0;
}


int codif_aux2(char *path, void *args) {
    char izq, der;
    int fd = open(path, O_RDWR);
    long m, n, filesize;

    /* Verifica que el archivo fue abierto */
    if (fd == -1) return -1;

    filesize = lseek(fd, -1, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    m = 0;
    n = filesize / 2;
    while (n) {
        /* Lee el caracter más izquierdo a intercambiar */
        if (lseek(fd, m++, SEEK_SET) == -1) return -1;
        if (read(fd, &izq, 1) == -1) return -1;

        /* Lee el caracter más derecho a intercambiar */
        if (lseek(fd, -m-1, SEEK_END) == -1) return -1;
        if (read(fd, &der, 1) == -1) return -1;

        /* Escribe el caracter más derecho en la izquierda*/
        if (lseek(fd, -m-1, SEEK_END) == -1) return -1;
        if (write(fd, &izq, 1) == -1) return -1;

        /* Escribe el caracter más izquierdo en la derecha*/
        if (lseek(fd, m-1, SEEK_SET) == -1) return -1;
        if (write(fd, &der, 1) == -1) return -1;

        n--;
    }

    /* Cierra el archivo */
    close(fd);
    return 0;
}



int cfind(char *path, struct Args *args) {
    FILE *stream;
    char *line = NULL;
    char *string = args->cadena1;
    char *string2 = args->cadena2;
    size_t len = 0;
    ssize_t nread;

    /* Verifica si path contiene el string */
    if (!strstr(path, string)) return 0;
    
    /* Abre el archivo regular */
    stream = fopen(path, "r");
    if (stream == NULL) {
        fprintf(stderr, "Error al abrir el archivo %s\n", path);
        return -1;
    }

    /* Verifica que el contenido del archivo tenga la cadena string2 */
    while ((nread = getline(&line, &len, stream)) != -1) {
        if (strstr(line, string2)) {
            printf("%s\n", path);
            free(line);
            fclose(stream);
            return 1;
        }
    }

    free(line);
    fclose(stream);
    return 0;
}

/* MALOOO */
int cfind(char *path, struct Args *args) {
    char *string = args->cadena1;
    char *string2 = args->cadena2;
    char buffer[BUFSIZ];
    int fd, len;

    /* Verifica si path contiene el string */
    if (!strstr(path, string)) return 0;
    
    /* Abre el archivo regular */
    fd = open(path, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "Error al abrir el archivo %s\n", path);
        return -1;
    }

    /* Verifica que el contenido del archivo tenga la cadena string2 */
    while ((len = read(fd, buffer, BUFSIZ)) > 0) {
        if (strstr(buffer, string2)) {
            printf("%s\n", path);
            return 1;
        }
    }

    close(fd);
    return 0;
}


int roll_aux2(char *path, struct Args *args) { 
    char *buffer_n, *buffer;
    int filesize;  
    int n = args->n;
    int m = abs(n);

    /* Verifica que el archivo fue abierto */
    int fd = open(path, O_RDWR);
    if (fd == -1) return -1;

    filesize = lseek(fd, -1, SEEK_END);
    buffer_n = (char*)malloc(sizeof(char) * n);
    buffer = (char*)malloc(sizeof(char) * (filesize - m));
    if (!buffer_n || !buffer) return -1;

    /* Si n es positivo, se rota hacia la derecha */
    if (n >= 0) {
        /* Lee los ultimos n caracteres */
        lseek(fd, -m-1, SEEK_END);
        read(fd, buffer_n, m);

        /* Rota n caracteres desde el principio */
        lseek(fd, 0, SEEK_SET);
        read(fd, buffer, filesize - m);

        /* Escribe los nuevos caracteres */
        lseek(fd, m, SEEK_SET);
        write(fd, buffer, filesize - m);
        lseek(fd, 0, SEEK_SET);
        write(fd, buffer_n, m);
    } else {
        /* Si n es negativo, se rota hacia la izquierda */
        
        /* Lee los primeros n caracteres */
        lseek(fd, 0, SEEK_SET);
        read(fd, buffer_n, m);

        /* Rota n caracteres desde el final */
        read(fd, buffer, filesize - m);

        /* Escribe los nuevos caracteres */
        lseek(fd, 0, SEEK_SET);
        write(fd, buffer, filesize - m);
        write(fd, buffer_n, m);
    }

    free(buffer_n);
    free(buffer);
    close(fd);

    return 0;
}

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
        char separator = ' ';
        char *cadena, *comando;
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
        char *token, *str;
        printf("myutil> ");
        str = get_line();
        if (!str) fprintf(stderr, "Error al leer la línea de comando\n");

        /* Separa los argumentos de la línea de comando */
        token = strtok(str, " ");
        if (!token) {
            free(str);
            continue;
        }
        
        /* Ejecuta la función correspondiente según el comando invocado */
        if (!strcmp(token, "find")) {
            char *cadena = strtok(NULL, " ");
            find(dir_raiz, cadena);
        } else if (!strcmp(token, "ifind")) {
            char *cadena = strtok(NULL, " ");
            ifind(dir_raiz, cadena);
        } else if (!strcmp(token, "cfind")) {
            char *cadena1 = strtok(NULL, " ");
            char *cadena2 = strtok(NULL, " ");
            char *cadena3 = strtok(NULL, " ");

            if (!cadena1 || !cadena2 || cadena3) {
                fprintf(stderr, "Error: El comando debe ser de la forma: cfind <cadena1> <cadena2>\n");
                free(str);
                continue;
            }

            cfind(dir_raiz, cadena1, cadena2);
        } else if (!strcmp(token, "repla")) {
            char *file = strtok(NULL, " ");
            char *wrong_file = strtok(NULL, " ");

            if (!file || wrong_file) {
                fprintf(stderr, "Error: El comando debe ser de la forma: repla <file>\n");
                free(str);
                continue;
            }

            repla(dir_raiz, file);
        } else if (!strcmp(token, "wc")) {
            if (strtok(NULL, " ")) {
                fprintf(stderr, "Error: El comando debe ser de la forma: wc\n");
                free(str);
                continue;
            }

            wc(dir_raiz);
        } else if (!strcmp(token, "codif")) {
            if (strtok(NULL, " ")) {
                fprintf(stderr, "Error: El comando debe ser de la forma: codif\n");
                free(str);
                continue;
            }

            codif(dir_raiz);
        } else if (!strcmp(token, "roll")) {
            int n = 0;
            char *number = strtok(NULL, " ");
            if (number) {
                if (!is_integer(number)) {
                    fprintf(stderr, "Error: El comando debe ser de la forma: roll [<n>] con n entero.\n");
                    free(str);
                    continue;
                }
                n = atoi(number);
            }

            roll(dir_raiz, n);
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
    len = read(ptr, &ch, 1);
    while (ch != EOF && len != 0) {   
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
                break;
            }

            /* En cambio, se revisa con la siguiente palabra de la lista */
            actual = actual->next;
            lseek(ptr, pos, SEEK_SET);
            if ((len = read(ptr, &ch, 1)) == 0) break;
        }

        /* Si no coincide ninguna palabra de la lista, se imprime el char */
        if (!actual) {
            write(write_ptr, &ch, 1);
            if ((len = read(ptr, &ch, 1)) == 0) break;
        } 
    }

    close(ptr);
    close(write_ptr);

    /* Renombra el archivo temporal, si no se puede, se elimina */
    if (rename(temp_archivo, archivo) != 0) {
        free(temp_archivo);
        remove(temp_archivo);
        return -1;
    }

    /* Copiar permisos del archivo original y liberar nombre del archivo temporal */
    chmod(archivo, st.st_mode);
    free(temp_archivo);

    return 0;
}