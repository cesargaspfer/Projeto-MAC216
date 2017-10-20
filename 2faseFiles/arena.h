/*#ifndef MAQ_H
/*#define MAQ_H*/
#include "maq.h"

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

/*#endif /* MAQ_H */
