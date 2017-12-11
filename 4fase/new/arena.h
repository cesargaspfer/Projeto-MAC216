#ifndef ARENA_H
#define ARENA_H
typedef enum {
  GRAMA,
  AREIA,
  PEDRA,
  GELO,
  AGUA,
  INGREME,
  BASE
} Terreno;

// Itens coletáveis existentes no jogo
typedef enum {
  NENHUM,
  CRISTAL,
  ARMA
} Coletavel;

typedef struct {
  Terreno terreno;
  // indica se a célula está vazia ou não
  // vazia: 0
  int vazia;
  // indica o número de cristais na célula
  int nCristais;
  // indica se a célula é uma base
  int base;
  // indica qual coletável existe na célula
  /**
  * 0: nenhum
    1: cristal
    2: arma
  **/
  Coletavel coletavel;
} Celula;

//int main ();
//void assignRobo(Maquina *m);
#endif
