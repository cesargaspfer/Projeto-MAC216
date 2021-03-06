#ifndef MAQ_H
#define MAQ_H
#include "pilha.h"
#define MAXMEM 100

#define MAXMEM 100
#define MAXFRM 30

typedef struct {
  Pilha pil;
  Pilha exec;
  OPERANDO Mem[MAXMEM];
  INSTR prog[2000];
  int ip;
  int bp[MAXFRM];
  int ib;

  // coordenadas do robô concretamente

  // coordenadas da célula em que está
  int posx;
  int posy;
  int crist;
  int energia;
  // exército do robô
  int exercito;
  // dano causado pelo robô quando ataca
  int dano;
  int vida;
} Maquina;

//Maquina *cria_maquina(INSTR *p, int posx, int posy, int exercito);
Maquina *cria_maquina(char *nome, int posx, int posy, int exercito);
void destroi_maquina(Maquina *m);

void exec_maquina(Maquina *m, int n);

int new_frame(Maquina *m, int pos);

int del_frame(Maquina *m);
#endif /* MAQ_H*/
