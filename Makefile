CC = gcc
CFLAGS = -ansi -Wall -pedantic -g

all: ejecutable

ejecutable: main.o finds.o repla.o wc.o codif_roll.o lista.o par.o utilidades.o
	$(CC) $(CFLAGS) -o myutil main.o finds.o repla.o wc.o codif_roll.o lista.o par.o utilidades.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

finds.o: finds.c finds.h
	$(CC) $(CFLAGS) -c finds.c

repla.o: repla.c repla.h
	$(CC) $(CFLAGS) -c repla.c

wc.o: wc.c wc.h
	$(CC) $(CFLAGS) -c wc.c

codif_roll.o: codif_roll.c codif_roll.h
	$(CC) $(CFLAGS) -c codif_roll.c

lista.o: lista.c lista.h
	$(CC) $(CFLAGS) -c lista.c

par.o: par.c par.h
	$(CC) $(CFLAGS) -c par.c

utilidades.o: utilidades.c
	$(CC) $(CFLAGS) -c utilidades.c

clean:
	rm *.o myutil