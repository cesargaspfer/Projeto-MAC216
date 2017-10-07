#include<stdio.h>
#include<stdlib.h>
#include<arena.h>

Celula arena[20][20];
Robo robos[2][5];

int main () {
  CriaArena(20, 2, 10, 5);
}

void Atualiza (){

}
void Sistema(int op) {

}

/*-------------------------------------------------------------------------------------*/
//                                                                                     //
//                                      Cria Robo                                      //
//                                                                                     //
/*-------------------------------------------------------------------------------------*/
void InsereExercito (int time, int posX, int posY, int qual) {
  robos[time-1][qual]->posx = posX;
  robos[time-1][qual]->posy = posY;
}

/*-------------------------------------------------------------------------------------*/
//                                                                                     //
//                                    Destroi Robo                                     //
//                                                                                     //
/*-------------------------------------------------------------------------------------*/
void RemoveExercito(int posX, int posY) {
  int time = 0;
  int qual = 0;
  for(int i = 0; i < robos.length; i++){
    for(int j = 0; i < robos[j].length; j++){
      if(robos[time-1][qual]->posx == posX && robos[time-1][qual]->posy == posY) {
        time = i;
        qual = j;
        break;
      }
    }
  }
  robos[time][qual]->posx = -1;
  robos[time][qual]->posy = -1;
  robos[time][qual]->vida = 0;
  for(int i = 0; i < robos[time][qual]->crist; i++){
    arena[posX][posY]->nCristais++;
  }
  robos[time][qual]->crist = 0;
}


int CriaArena(int tamanho, int times, int cristais, int robos){

  // Gera a Base do terreno
  for(int i = 0; i < 20; i++){
    for(int j = 0; j < 20; j++){
      arena[i][j]->terreno = 0;
      arena[i][j]->vazia = 0;
      arena[i][j]->nCristais = 0;
      arena[i][j]->base = 0;
    }
  }

  // Gera de modo aleatorio o terreno, com irregularidades
  for(int a = 0; a < 7; a++){
    int tipoterr = rand() % 5;
    int localX = rand() % 20;
    int localY = rand() % 20;
    int tamanhoX = rand() % 5;
    int tamanhoY = rand() % 5;
    for(int i = localX; i < Minimo(20, localX + tamanhoX); i++) {
      for(int j = localY; j < Minimo(20, localY + tamanhoY); j++) {
        arena[i][j]->terreno = tipoterr;
      }
    }
  }

  //Bota um cristal no meio da arena, sendo no terreno mais dificil
  for(int i = (tamanho/2) - 3; i <= (tamanho/2) + 3; i++){
    for(int j = (tamanho/2) - 3; j <= (tamanho/2) + 3; j++){
      arena[i][j]->terreno = 4;
    }
  }
  arena[tamanho/2][tamanho/2]->nCristais++;

  // Gera as bases aleatoriamente
  for(int i = 1; i <= times; i++){
    int localX = rand() % 20;
    int localY = rand() % 20;
    if(arena[localX][localY]->base){
      i--;
    }
    else{
      arena[localX][localY]->base = i;
    }
  }

  // Bota os cristais aleatoriamente
  for(int i = 1; i < cristais; i++){
    int localX = rand() % 20;
    int localY = rand() % 20;
    if(arena[localX][localY]->base){
      i--;
    }
    else{
      arena[localX][localY]->nCristais++;
    }
  }

  // Bota os robos aleatoriamente
  for(int j = 1; j <= times; j++){
    for(int i = 0; i < robos; i++){
      int localX = rand() % 20;
      int localY = rand() % 20;
      if(arena[localX][localY]->vazia){
        i--;
      }
      else{
        arena[localX][localY]->vazia++;

        InsereExercito(times, localX, localY, i);

      }
    }
  }
}

static int Maximo (int a, int b){
  if(a > b) return a;
  else return b;
}
static int Minimo (int a, int b){
  if(a < b) return a;
  else return b;
}
