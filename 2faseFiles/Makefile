CC = gcc
CFLAGS = -g -Wall

default: arena

arena: maq.o pilha.o arena.o
	$(CC) $(CFLAGS) -o arena maq.o arena.o pilha.o

clean:
	rm -f arena *.o
