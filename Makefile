CC = gcc
CFLAGS = -ansi -Wall -pedantic -g

all: ejecutable

ejecutable: main.o finds.o repla.o lista.o par.o utilidades.o
	$(CC) $(CFLAGS) -o myutil main.o finds.o repla.o lista.o par.o utilidades.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

finds.o: finds.c finds.h
	$(CC) $(CFLAGS) -c finds.c

repla.o: repla.c repla.h
	$(CC) $(CFLAGS) -c repla.c

lista.o: lista.c lista.h
	$(CC) $(CFLAGS) -c lista.c

par.o: par.c par.h
	$(CC) $(CFLAGS) -c par.c

utilidades.o: utilidades.c
	$(CC) $(CFLAGS) -c utilidades.c

clean:
	rm *.o myutil