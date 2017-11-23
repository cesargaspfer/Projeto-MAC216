#include "arena.h"
#include "maq.h"
typedef struct {
  Celula arena[15][15];
  Maquina *robos[2][5];
  int RobosAtivos[2][5];
} Arena;

int coleta(int x, int y, Maquina *m, Arena *ArenaSt);
int ataque(int x, int y, Maquina *m, Arena *ArenaSt);
int move(int x, int y, Maquina *m, Arena *ArenaSt, int timeAtual, int roboAtual);
void inicializaRobo (int index, int time, int posX, int posY, Arena *ArenaSt);
void destroiRobo(int posX, int posY, int time, int qual, Arena *ArenaSt);
Arena CriaArena(int tamanho, int times, int cristais, int robosT, int armas);
