#include "pilha.h"
#define MAXMEM 100

typedef struct {
  Pilha pil;
  Pilha exec;
  OPERANDO Mem[MAXMEM];
  INSTR *prog;
  int ip;
  int posx;
  int posy;
  int vida;
  int crist;
  int time;
} Maquina;

Maquina *cria_maquina(INSTR *p, int posX, int posY, int vidaV, int CrstV, int TimeV);

void destroi_maquina(Maquina *m);

void exec_maquina(Maquina *m, int n);
// declaração de função para retornar o array de instruções
INSTR[] getProg();
