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
} Maquina;

// declaração da função cria_maquina
<<<<<<< HEAD
Maquina *cria_maquina(INSTR *p, int posX, int posY, int vidaV, int CrstV, int TimeV, int energia);
=======
Maquina *cria_maquina(INSTR *p, int posX, int posY, int vidaV, int CrstV, int TimeV, int energ);
>>>>>>> 1054d1e65a9558d49d7e416b3ebd6e57b0c54325
// declaração da função destroi_maquina
void destroi_maquina(Maquina *m);
// declaração da função exec_maquina
void exec_maquina(Maquina *m, int n);

INSTR *geraProg();


void Sistema(int op, int dir,  Maquina *m);
void Atualiza ();
#endif /* MAQ_H*/
