CC = gcc
CFLAGS = -g -Wall

default: arena

arena: maq.o pilha.o arena.o
	$(CC) $(CFLAGS) -o arena maq.o arena.o pilha.o

maq.o: maq.c maq.h pilha.h instr.h
	$(CC) $(CFLAGS) -c maq.c

pilha.o: pilha.c pilha.h instr.h util.h
	$(CC) $(CFLAGS) -c pilha.c

arena.o: arena.c arena.h
	$(CC) $(CFLAGS) -c arena.c

clean:
	rm -f arena *.o
