#ifndef ARE_H
#define ARE_H

#include "maq.h"

typedef struct {
  int terreno;
  int vazia;
  int nCristais;
  int base;
} Celula;

//int main ();
void assignRobo(Maquina *m);
void Sistema(int op, int dir,  Maquina *m);
void Atualiza ();
#endif /* ARE_H */
