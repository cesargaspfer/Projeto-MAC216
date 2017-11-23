#include<stdio.h>
#include<stdlib.h>
#include "arena.h"
#include "arenaMaster.h"
#include "maq.h"
#include "tprog.h"
#include<time.h>
#include "util.h"
#include "controle.h"

/*-------------------------------------------------------------------------------------*/
//                                                                                     //
//                                       Coleta                                        //
//                                                                                     //
/*-------------------------------------------------------------------------------------*/
int coleta(int x, int y, Maquina *m, Arena *ArenaSt){
  // Caso existam coletáveis na célula em (x, y):
  if(ArenaSt->arena[x][y].coletavel != NENHUM){
    Coletavel c = ArenaSt->arena[x][y].coletavel;
    switch (c)
    {
      case CRISTAL:
        // diminui o número de cristais na célula
        ArenaSt->arena[x][y].nCristais--;
        // se foi pego o último cristal
        if (ArenaSt->arena[x][y].nCristais == 0)
          // não há mais nenhum coletável
          ArenaSt->arena[x][y].coletavel = NENHUM;
        // o robô está carregando mais um executável
        m->crist++;
        break;
      case ARMA:
        ArenaSt->arena[x][y].coletavel = NENHUM;
        // o dano do robô aumenta para 30 (um ataque dele tira 30 pontos de saúde da vítima)
        // se ele já tinha uma arma, o dano aumenta para 60!
        if (m->dano == 30)
          m->dano = 60;
        else
          m->dano = 30;
        break;
      default:
        break;
    }
	   // Retorna sucesso
     return 1;
  }
  // Caso nao tenha coletáveis na célula
  else{
	// Retorna fracasso
    return 0;
  }
}

/*-------------------------------------------------------------------------------------*/
//                                                                                     //
//                                       Ataque                                        //
//                                                                                     //
/*-------------------------------------------------------------------------------------*/
int ataque(int x, int y, Maquina *m, Arena *ArenaSt){
  // Caso tenha robo para atacar na posicao desejada:
  if(ArenaSt->arena[x][y].vazia != 0){
    m->energia += 3;
    // Procura o robo nessa posicao
    int time = -1;
    int qual = -1;
    for(int i = 0; i < 2; i++){
      if (time != -1 && qual != -1)
        break;
      for(int j = 0; i < 5; j++){
        if(ArenaSt->robos[i][j]->posx == x && ArenaSt->robos[i][j]->posy == y) {
          time = i;
          qual = j;
        }
        if (time != -1 && qual != -1)
          break;
      }
    }
    //toca som de ataque
    mostraAtaque();
    // Retira pontos de vida do robo atacado em função do dano associado ao robô atacante
    ArenaSt->robos[time][qual]->vida -= m->dano;
    printf("Saúde do robô atacado: %d\n", ArenaSt->robos[time][qual]->vida);
    // Caso acabe a vida desse robo, ele será destruido
    if(ArenaSt->robos[time][qual]->vida <= 0){
      // remove o robô de seu exército
      destroiRobo(x, y, time, qual, ArenaSt);
    }
    // Retorna sucesso
    return 1;
  }
  // Caso esteja vazia:
  else{
    // Retorna fracasso
    return 0;
  }
}

/*-------------------------------------------------------------------------------------*/
//                                                                                     //
//                                        Move                                         //
//                                                                                     //
/*-------------------------------------------------------------------------------------*/
int move(int x, int y, Maquina *m, Arena *ArenaSt, int timeAtual, int roboAtual){

  // Tem alguem ai? - Verifica se ja tem um robo na celula desejada
  if(ArenaSt->arena[x][y].vazia != 0){

    // Recusa a solicitação
    return 0;
  }
  // Se não houver robô na celula desejada
  else {
    // salva coordenadas "antigas" do robô
    int ox = m->posx;
    int oy = m->posy;
    // Muda estado da arena
    // Marca a celula que o robo está deixando para vazia
    ArenaSt->arena[ox][oy].vazia = 0;
    // Marca a celula que o robo esta indo para nao vazia, indicando o seu time
    ArenaSt->arena[x][y].vazia =  timeAtual + 1;
    // Muda estado do robo
    // Muda sua posicao
    moveRobo(roboAtual + (timeAtual)*5, x, y);

    m->posx = x;
    m->posy = y;
    m->energia = ArenaSt->arena[x][y].terreno;
    // se na célula pela qual o robô passou havia algum item que não foi coletado
    if (ArenaSt->arena[ox][oy].coletavel != NENHUM)
    {
      // redesenha o item
      if (ArenaSt->arena[ox][oy].coletavel == CRISTAL)
        desenhaCristal(1, ox, oy);
      else if (ArenaSt->arena[ox][oy].coletavel == ARMA)
        desenhaArma(ox, oy);
        //Se havia uma base
    } else if (ArenaSt->arena[ox][oy].base != 0)
    {
      // desenha a base novamente
      desenhaBase(ox,oy,ArenaSt->arena[ox][oy].base);
    }
    // Retorna sucesso
    return 1;
  }
}

/*-------------------------------------------------------------------------------------*/
//                                                                                     //
//                                      Cria Robo                                      //
//                                                                                     //
/*-------------------------------------------------------------------------------------*/
void inicializaRobo (int index, int time, int posX, int posY, Arena *ArenaSt) {

  printf("inicializou robô %d no time %d na posição (%d , %d)\n", index, time, posX, posY);
  // Cria o robo com a função cria_maquina
  ArenaSt->robos[time][index] = cria_maquina(prog, posX, posY, time);
  // Marca como "ativo" esse robo no vetor de robos ativos
  ArenaSt->RobosAtivos[time][index] = 1;
}

/*-------------------------------------------------------------------------------------*/
//                                                                                     //
//                                    Destroi Robo                                     //
//                                                                                     //
/*-------------------------------------------------------------------------------------*/
void destroiRobo(int posX, int posY, int time, int qual, Arena *ArenaSt) {

  //Caso não foi informado de qual time o robo pertence
  if(time == -1){
    // Procura o robo a destruir, pela sua posição
    time = 0;
    qual = 0;
    for(int i = 0; i < 2; i++){
      for(int j = 0; i < 5; j++){
        if(ArenaSt->robos[time][qual]->posx == posX && ArenaSt->robos[time][qual]->posy == posY) {
          time = i;
          qual = j;
          break;
        }
      }
    }
  }
  int terreno = ArenaSt->arena[ArenaSt->robos[time][qual]->posx][ArenaSt->robos[time][qual]->posy].terreno;
  desenhaCelula(ArenaSt->robos[time][qual]->posx, ArenaSt->robos[time][qual]->posy, terreno);
  if(ArenaSt->arena[ArenaSt->robos[time][qual]->posx][ArenaSt->robos[time][qual]->posy].nCristais){
    desenhaCristal(1, ArenaSt->robos[time][qual]->posx, ArenaSt->robos[time][qual]->posy);
  }
  // "Destroi" o robo no vetor
  ArenaSt->robos[time][qual]->posx = -1;
  ArenaSt->robos[time][qual]->posy = -1;
  ArenaSt->robos[time][qual]->vida = 0;

  // Caso ele tenha cristais, ele os derruba (para uma posicao aleatoria, mas perto dele)
  for(int i = 0; i < ArenaSt->robos[time][qual]->crist; i++){
    int localX = rand() % 3 -1;
    int localY = rand() % 3 -1;
    int cairX = posX + localX;
    int cairY = posY + localY;

    //Verifica se nao caiu dentro da arena, caso nao tenha caido, bota na extremidade da arena
         if (cairX < 0)  cairX = 0;
    else if (cairX > 19) cairX = 19;
         if (cairY < 0)  cairY = 0;
    else if (cairY > 19) cairY = 19;
    ArenaSt->arena[posX][posY].nCristais++;
    ArenaSt->arena[posX][posY].coletavel = CRISTAL;
    // desenha 1 cristal na célula (x,y)
    desenhaCristal(1, posX, posY);
  }
  ArenaSt->robos[time][qual]->crist = 0;

  // Marca como "inativo" esse robo no vetor de robos ativos
  ArenaSt->RobosAtivos[time][qual] = 0;
}

/*-------------------------------------------------------------------------------------*/
//                                                                                     //
//                                     Cria Arena                                      //
//                                                                                     //
/*-------------------------------------------------------------------------------------*/
Arena CriaArena(int tamanho, int times, int cristais, int robosT, int armas){

  Arena ArenaSt;
  Arena ArenaSt2;

  printf("%d\n", ArenaSt2.arena[0][0].terreno);



  Celula campo[14][14];
  //memcpy (  ArenaSt->arena, campo, 225 );
  //printf("%d\n", ArenaSt->arena[0][0]);
  //memcpy(ArenaSt->arena, campo, sizeof(ArenaSt->arena));
  /*
  for(int i = 0; i < times; i++){
    pontosTotais[i] = 0;
  }
  */

  /*
  fimDoJogo = 0; // O jogo não terminou

   timeAtual = 0;
	 roboAtual = 0;
   */

   /*
  time_t t;
  srand((unsigned) time(&t));
  */

  // Gera a arena, definindo aleatoriamente seus terrenos
  for(int i = 0; i < 15; i++){
   for(int j = 0; j < 15; j++){
     // define um dos cinco terrenos ( de 0 a 4)
     ArenaSt.arena[i][j].terreno = rand() % 5;
     // inicializa sempre vazia
     ArenaSt.arena[i][j].vazia = 0;
     // inicializa sem nenhum cristal, pois é preciso definir se a celula em questão é base ou não
     ArenaSt.arena[i][j].nCristais = 0;
     // por enquanto, não a cristais em nenhuma das células
     ArenaSt.arena[i][j].coletavel = NENHUM;
   }
 }

 //Inicializa a construção da visualização
 inicializaGraf();
 // envia a arena para a visualização, sem robôs ou coletáveis ainda
 for (int i = 0; i < tamanho; i++)
 {
   for (int j = 0; j < tamanho; j++){
     Celula c = ArenaSt.arena[i][j];
     desenhaCelula(i, j, c.terreno);
   }
 }

 // define as duas bases de cada time e as desenha
 for (int i = 1; i <= times; i++)
 {
   // sorteia a posição da base (a primeira na metade superior da arena, a segunda na metade superior)
   int x = rand()%(7) + 7*(i-1);
   int y = rand()%(7) + 7*(i-1);
   if (ArenaSt.arena[x][y].base)
   {
    i--;
    continue;
   }

   // define como verdadeiro na struct que a célula é uma base
   ArenaSt.arena[x][y].base = i;
   // define o "tipo de terreno" como BASE
   ArenaSt.arena[x][y].terreno = BASE;
   desenhaBase(x,y,i);
 }


  // Distribuir cristais pela arena, de modo a não colocar um cristal onde já tem um robô ou
  // uma base e os desenha
  for (int i = 0; i < cristais; i++)
  {

      int x = rand()%14;
      int y = rand()%14;
      if (ArenaSt.arena[x][y].vazia != 0 || ArenaSt.arena[x][y].base != 0)
        i--;
      else
      {
        ArenaSt.arena[x][y].nCristais++;
        ArenaSt.arena[x][y].coletavel = CRISTAL;
        // desenha 1 cristal na célula (x,y)
        desenhaCristal(1, x, y);
      }
  }

  // Distribuir duas armas na arena em células não ocupadas por bases ou cristais
  for (int i = 0; i < armas;)
  {
    int x = rand()%14;
    int y = rand()%14;
    if (ArenaSt.arena[x][y].vazia == 0 && ArenaSt.arena[x][y].coletavel == NENHUM)
    {
      // define o coletável da célula(x,y) como ARMA
      ArenaSt.arena[x][y].coletavel = ARMA;
      // desenha uma arma na posição (x,y)
      desenhaArma(x,y);
      i++;
    }
  }

  /*
  // printar a arena. Util para debug
  for(int i = 0; i < 15; i++){
    for(int j = 0; j < 15; j++){
        printf("%d ", arena[i][j].terreno);
    }
    printf("\n");
  }
  */

  // Bota os robos aleatoriamente no mapa
  int index = 0;
  for(int j = 1; j <= times; j++){
    for(int i = 0; i < robosT; i++){
      // os robos do primeiro time começam sempre na metade superior da arena, onde está sua base
      // já os do segundo time começarão o jogo na metade inferior da arena
      int localX = rand()%7 + 7*(j-1);
      int localY = rand() %7 + 7*(j-1);
      // Caso ja tenha um robo, uma base ou um item coletavel no local sorteado, decrementa o i para refazer esse loop
      if(ArenaSt.arena[localX][localY].vazia || ArenaSt.arena[localX][localY].base || ArenaSt.arena[localX][localY].coletavel != NENHUM){
        i--;
      }
      // Se estiver vazia:
      else{
        // marca a arena como ocupada (vazia = 0, sennao representa qual time esta ai)
        ArenaSt.arena[localX][localY].vazia = j;
        // desenha o robô "index" do exército j
        desenhaRobo(index, j-1, localX, localY);
        // coloca o robô i no time j
        inicializaRobo(i, j-1, localX, localY, &ArenaSt);
        index++;
      }
    }
  }
  return ArenaSt;
}


/*-------------------------------------------------------------------------------------*/
//                                                                                     //
//                                       Maximo                                        //
//                                                                                     //
/*-------------------------------------------------------------------------------------*/
// Retorna o maior valor entre dois numeros
int Maximo (int a, int b){
  if(a > b) return a;
  else return b;
}

/*-------------------------------------------------------------------------------------*/
//                                                                                     //
//                                       Minimo                                        //
//                                                                                     //
/*-------------------------------------------------------------------------------------*/
// Retorna o menor valor entre dois numeros
int Minimo (int a, int b){
  if(a < b) return a;
  else return b;
}
