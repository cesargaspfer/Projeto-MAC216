
#include "maq.h"

#ifndef ARE_H
#define ARE_H
#endif /* ARE_H */

typedef struct {
  int terreno;
  int vazia;
  int nCristais;
  int base;
} Celula;

typedef struct {
  int posx;
  int posy;
  int vida;
  int crist;
  int time;
} Robo;

int main ();
void Sistema(int op, int dir, Maquina *m);
void Atualiza ();
