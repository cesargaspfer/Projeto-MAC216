#ifndef ARENA_H
#define ARENA_H
typedef enum {
  ELEVADO,
  GRAMA,
  AREIA,
  PEDRA,
  GELO
} Terreno;

typedef struct {
  Terreno terreno;
  int vazia;
  int nCristais;
  int base;
} Celula;
//int main ();
//void assignRobo(Maquina *m);
#endif
