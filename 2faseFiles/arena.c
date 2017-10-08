#include<stdio.h>
#include<stdlib.h>
#include<arena.h>
#include<time.h>

// Duas variáveis locais: a arena e os robos
Celula arena[20][20];
Robo robos[2][5];
int cristaisRestantes [20];
int pontosTotais [2];

int main () {
  CriaArena(20, 2, 10, 5);
}

/*-------------------------------------------------------------------------------------*/
//                                                                                     //
//                                       Sistema                                       //
//                                                                                     //
/*-------------------------------------------------------------------------------------*/
void Sistema(int op) {
  clock_t begin = clock();
  int movX = 0;
  int movY = 0;

  /*
  Legenda da direcao:
     0 - Norte
     1 - Nordeste
     2 - Sudeste
     3 - Sul
     4 - Sudoeste
     5 - Noroeste
  */

  // Ajusta o movimento da matriz exagonal para a matriz quadrada
  if(dir == 1){
    movX = 1;
    movY = 1;
  }
  else if(dir == 2){
    movX = 1;
  }
  else if(dir == 4){
    movX = -1;
  }
  else if(dir == 5){
    movX = -1;
    movY = 1;
  }
  if(robos[time-1][robo]->posx%2){
    movY--;
  }
  if(dir == 0){
    movY = 1;
  }
  else if(dir == 3){
    movY = -1;
  }
  // Posicao atual
  else if (dir == 6){
    movX = 0;
    movY = 0;
  }
  // Futura possivel posicao do robo
  int posTmpX = robos[time-1][robo]->posx + movX;
  int posTmpY = robos[time-1][robo]->posy + movY;

  //Fora do mapa?
  if(posTmpX < 0 || posTmpY < 0 || posTmpX > 19 || posTmpY > 19) {
    //empilhaNoRobo falso
  }
  else{
    /*
    Se Informacao {
      empilhaNoRobo(arena[posTmpX][posTmpY]);
    }
    Se move {
      if(move(qual robo do time, qual time, posTmpX, posTmpY) != 0)
        empilhaNoRobo falso
    }
    Se Ataque{
      ataque(posTmpX, posTmpY);
    }
    Se Coletar{
      if(coleta(posTmpX, posTmpY))
        robos[time-1][robo]->crist++;
      else
        empilhaNoRobo falso
    }
    Se Depositar {
      if(robos[time-1][robo]->crist) {
        if(arena[posTmpX][posTmpY]->base){
          pontosTotais[arena[posTmpX][posTmpY]->base -1]++;
          cristaisRestantes--;
          if(cristaisRestantes == 0)
            Fim();
        }
        else {
           arena[posTmpX][posTmpY]->nCristais++;
        }
        robos[time-1][robo]->crist--;
      }
      else
        empilhaNoRobo falso
    }
    */
  }
}

/*-------------------------------------------------------------------------------------*/
//                                                                                     //
//                                        Fim                                          //
//                                                                                     //
/*-------------------------------------------------------------------------------------*/
void Fim (clock_t begin) {
  clock_t end = clock();
  double currentTime = (double) (end - begin)/ CLOCKS_PER_SEC;
}

/*-------------------------------------------------------------------------------------*/
//                                                                                     //
//                                      Atualiza                                       //
//                                                                                     //
/*-------------------------------------------------------------------------------------*/
void Atualiza (){
  // Tempo trasncorrido - nao esquecer
  /*
  for (int i = 0; i < 10; i++) {
      move(robo[i],)
  }
  */
}

/*-------------------------------------------------------------------------------------*/
//                                                                                     //
//                                       Coleta                                        //
//                                                                                     //
/*-------------------------------------------------------------------------------------*/
static int coleta(int posTmpX, int posTmpY){
  if(arena[posTmpX][posTmpY]->nCristais){
    arena[posTmpX][posTmpY]->nCristais--;
    return 1;
  }
  else{
    return 0;
  }
}

/*-------------------------------------------------------------------------------------*/
//                                                                                     //
//                                       Ataque                                        //
//                                                                                     //
/*-------------------------------------------------------------------------------------*/
static int ataque(int posTmpX, int posTmpY){
  if(arena[posTmpX][posTmpY]->vazia){
    // Procura o robo nessa posicao
    int time = 0;
    int qual = 0;
    for(int i = 0; i < robos.length; i++){
      for(int j = 0; i < robos[j].length; j++){
        if(robos[time][qual]->posx == posTmpX && robos[time][qual]->posy == posTmpY) {
          time = i;
          qual = j;
          break;
        }
      }
    }
    // Como, por enquanto so tem um tipo de ataque, deixemos essa parte comentada
    // if(tipo == 1)
    robos[time][qual]->vida -= 30;
    if(robos[time][qual]->vida <= 0){
      RemoveExercito(posTmpX, posTmpY, time, qual);
    }
    return 1;
  }
  else{
    return 0;
  }
}

/*-------------------------------------------------------------------------------------*/
//                                                                                     //
//                                        Move                                         //
//                                                                                     //
/*-------------------------------------------------------------------------------------*/
static int move(int robo, int time, int posTmpX, int posTmpY){

  // Tem alguem ai? -- possivelmente adicionar se tem base
  if(arena[posTmpX][posTmpY]->vazia != 0){
    return 0;
  }
  else {
    // Muda estado da arena
    arena[robos[time-1][robo]->posx][robos[time-1][robo]->posy]->vazia = 0;
    arena[posTmpX][posTmpY]->vazia = time;
    // Muda estado do robo
    robos[time-1][robo]->posx = posTmpX;
    robos[time-1][robo]->posy = posTmpY;
    return 1;
  }
}

/*-------------------------------------------------------------------------------------*/
//                                                                                     //
//                                      Cria Robo                                      //
//                                                                                     //
/*-------------------------------------------------------------------------------------*/
static void InsereExercito (int time, int posX, int posY, int qual) {
  robos[time-1][qual]->posx = posX;
  robos[time-1][qual]->posy = posY;
  robos[time-1][qual]->vida = 100;
  robos[time-1][qual]->crist = 0;
  robos[time-1][qual]->time = time;
  /////////////////////////////////////////////Criar a maquina lah//////////////////////////////////////////
}

/*-------------------------------------------------------------------------------------*/
//                                                                                     //
//                                    Destroi Robo                                     //
//                                                                                     //
/*-------------------------------------------------------------------------------------*/
static void RemoveExercito(int posX, int posY, int time, int qual) {

  if(time == -1){
    // Procura o robo a destruir, pela sua posição
    time = 0;
    qual = 0;
    for(int i = 0; i < robos.length; i++){
      for(int j = 0; i < robos[j].length; j++){
        if(robos[time][qual]->posx == posX && robos[time][qual]->posy == posY) {
          time = i;
          qual = j;
          break;
        }
      }
    }
  }

  // "Destroi" o robo no vetor
  robos[time][qual]->posx = -1;
  robos[time][qual]->posy = -1;
  robos[time][qual]->vida = 0;

  // Caso ele tenha cristais, ele os derruba
  for(int i = 0; i < robos[time][qual]->crist; i++){
    int localX = rand() % 3 -1;
    int localY = rand() % 3 -1;
    int cairX = posX + localX;
    int cairY = posY + localY;

    //Verifica se caiu dentro da arena
         if (cairX < 0)  cairX = 0;
    else if (cairX > 19) cairX = 19;
         if (cairY < 0)  cairY = 0;
    else if (cairY > 19) cairY = 19;
    arena[posX][posY]->nCristais++;
  }
  robos[time][qual]->crist = 0;
  ////////////////////////////////////////////////////////Destruir a maquina lah//////////////////////////qq
}

/*-------------------------------------------------------------------------------------*/
//                                                                                     //
//                                     Cria Arena                                      //
//                                                                                     //
/*-------------------------------------------------------------------------------------*/
static int CriaArena(int tamanho, int times, int cristais, int robos){

  int cristaisRestantes [cristais];
  int pontosTotais [times];


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
    if(arena[localX][localY]->base || arena[localX][localY]->nCristais){
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


/*-------------------------------------------------------------------------------------*/
//                                                                                     //
//                                       Maximo                                        //
//                                                                                     //
/*-------------------------------------------------------------------------------------*/
static int Maximo (int a, int b){
  if(a > b) return a;
  else return b;
}

/*-------------------------------------------------------------------------------------*/
//                                                                                     //
//                                       Minimo                                        //
//                                                                                     //
/*-------------------------------------------------------------------------------------*/
static int Minimo (int a, int b){
  if(a < b) return a;
  else return b;
}
