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
  // coordenada da célula em que está
  int posx;
  int posy;
  // número de cristais que carrega
  int crist;
  int energia;
  // time ao qual pertence
  int time;
  // dano que o robô causa quando ataca outro robô (decréscimo na "vida" de outro robô)
  // padrão: 10
  int dano;
} Maquina;

// declaração da função cria_maquina
Maquina *cria_maquina(INSTR *p, int posX, int posY, int TimeV);
void destroi_maquina(Maquina *m);
// declaração da função exec_maquina
void exec_maquina(Maquina *m, int n);

INSTR *geraProg();


void Sistema(int op, int dir,  Maquina *m);
void Atualiza ();
#endif /* MAQ_H*/
