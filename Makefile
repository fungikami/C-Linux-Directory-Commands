CC = gcc
CFLAGS = -ansi -Wall -pedantic -g

all: ejecutable

ejecutable: main.o utilidades.o
	$(CC) $(CFLAGS) -o myutil main.o utilidades.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

utilidades.o: utilidades.c
	$(CC) $(CFLAGS) -c utilidades.c

clean:
	rm *.o myutil