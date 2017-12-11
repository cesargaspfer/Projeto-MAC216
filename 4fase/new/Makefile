CC = gcc
CFLAGS = -g -w
default: game

game: maq.o pilha.o arena.o controle.o compila.tab.o
	$(CC) $(CFLAGS) maq.o pilha.o arena.o controle.o compila.tab.o -o game

maq.o: maq.c maq.h pilha.h instr.h
	$(CC) $(CFLAGS) -c maq.c

pilha.o: pilha.c pilha.h instr.h util.h
	$(CC) $(CFLAGS) -c pilha.c

arena.o: arena.c arena.h
	$(CC) $(CFLAGS) -c arena.c

controle.o: controle.c controle.h
	$(CC) $(CFLAGS) -c controle.c

compila.tab.o: compila.y
	bison -d compila.y
	gcc -c compila.tab.c

lex.yy.o: compila.l
	flex compila.l
	gcc -c lex.yy.c
clean:
	rm -f *.o lex.yy.c compila.tab.c compila.tab.h *~ game
