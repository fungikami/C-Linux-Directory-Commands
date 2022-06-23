CC = gcc
CFLAGS = -ansi -Wall -pedantic

all: cp

cp: main.o
	$(CC) $(CFLAGS) -o cp main.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

clean:
	rm *.o