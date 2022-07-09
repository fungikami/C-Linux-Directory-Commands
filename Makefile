CC = gcc
CFLAGS = -ansi -Wall -pedantic

all: ejecutable

ejecutable: main.o finds.o repla.o wc.o codif.o roll.o lista.o par.o misc.o
	$(CC) $(CFLAGS) -o myutil main.o finds.o repla.o wc.o codif.o roll.o lista.o par.o misc.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

finds.o: finds.c finds.h
	$(CC) $(CFLAGS) -c finds.c

repla.o: repla.c repla.h
	$(CC) $(CFLAGS) -c repla.c

wc.o: wc.c wc.h
	$(CC) $(CFLAGS) -c wc.c

codif.o: codif.c codif.h
	$(CC) $(CFLAGS) -c codif.c

roll.o: roll.c roll.h
	$(CC) $(CFLAGS) -c roll.c

lista.o: lista.c lista.h
	$(CC) $(CFLAGS) -c lista.c

par.o: par.c par.h
	$(CC) $(CFLAGS) -c par.c

misc.o: misc.c
	$(CC) $(CFLAGS) -c misc.c

clean:
	rm *.o myutil