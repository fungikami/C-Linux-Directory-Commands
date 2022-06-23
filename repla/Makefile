CC = gcc
CFLAGS = -ansi -Wall -pedantic

all: ejecutable

ejecutable: main.o repla.o lista.o par.o
	$(CC) $(CFLAGS) -o testrepla main.o repla.o lista.o par.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c
	
repla.o: repla.c repla.h
	$(CC) $(CFLAGS) -c repla.c

lista.o: lista.c lista.h
	$(CC) $(CFLAGS) -c lista.c

par.o: par.c par.h
	$(CC) $(CFLAGS) -c par.c

clean:
	rm *.o testrepla