#ifndef MAQ_H
#define MAQ_H
#include "pilha.h"
#define MAXMEM 100

typedef struct {
  Pilha pil;
  Pilha exec;
  OPERANDO Mem[MAXMEM];
  INSTR *prog;
  int ip;
  // atributos concretos de um robo //
  // saúde (diminui com avarias)
  int vida;
  // energia do robô (pode ser interpretado como combustível)
  int energia;
  // coordenada da célula em que está
  int posx;
  int posy;
  // número de cristais que carrega
  int crist;
  // time ao qual pertence
  int time;
} Maquina;

// declaração da função cria_maquina
Maquina *cria_maquina(INSTR *p, int posX, int posY, int vidaV, int CrstV, int TimeV);
// declaração da função destroi_maquina
void destroi_maquina(Maquina *m);
// declaração da função exec_maquina
void exec_maquina(Maquina *m, int n);

INSTR *geraProg();


void Sistema(int op, int dir,  Maquina *m);
void Atualiza ();
#endif /* MAQ_H*/
