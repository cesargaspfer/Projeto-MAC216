#include<stdio.h>
#include<stdlib.h>
#include<arena.h>
#include<time.h>

// Variáveis globais
int TotTimes = 2;
int TotRobTime = 5;
Celula arena[20][20];    // A arena em sí (o campo de batalha)
Maquina robos[2][5];        // Vetor que contem os estados dos robos
int cristaisRestantes;   // Cristais restantes na arena
int pontosTotais [2];    // Pontos totais de cada time
int timeAtual = 0;
int roboAtual = 0;
int RodadaAtual = 1;
int TempoDeCadaRobo[2][5];
int RobosAtivos[2][5];
clock_t begin;

int main () {
  CriaArena(20, TotTimes, 10, TotRobTime);
  timeAtual = 0;
  roboAtual = 0;
  RodadaAtual = 1;
  begin = clock();
  Atualiza();
}

/*-------------------------------------------------------------------------------------*/
//                                                                                     //
//                                       Sistema                                       //
//                                                                                     //
/*-------------------------------------------------------------------------------------*/

/* spec determina o que será feito, dado a op */

/*************************************************
* As op's vão ser implementadas da seguinte maneira,
* MOV = valor 0 (movimenta)
* ATK = valor 1 (ataca)
* INF = valor 2 (informação) | spec = 0;
* CLT = valor 3 (coleta)
* DEP = valor 4 (deposita)
*
*************************************************/

/*spec foi passado como 0 = valor significa que a instrução
só tem um tipo de reação. Exemplo: CLT (coleta) só determina uma
ação, logo chama o Sistema(3, 0). Já MOV, é diferente, pois pode
ser em várias direções. */
void Sistema(int op, int dir, Maquina *m) {

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
	 6 - Atual
  */

  // Ajusta o movimento da matriz hexagonal para a matriz quadrada
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
  if(robos[timeAtual][roboAtual]->posx%2){
    movY--;
  }
  if(dir == 0){
    movY = 1;
  }
  else if(dir == 3){
    movY = -1;
  }
  else if (dir == 6){
    movX = 0;
    movY = 0;
  }
  // Futura possivel posicao do robo
  int posTmpX = robos[timeAtual][roboAtual]->posx + movX;
  int posTmpY = robos[timeAtual][roboAtual]->posy + movY;

  //Fora do mapa?
  if(posTmpX < 0 || posTmpY < 0 || posTmpX > 19 || posTmpY > 19) {
    empilha(&m->pil, (OPERANDO){BOOL, false}); //empilhaNoRobo false
  }
  else{
    if(op == 2) { // Informação
      empilha(&m->pil, (OPERANDO){CELL, arena[posTmpX][posTmpY]}); // empilhaNoRobo Celula
    }
    else if(op == 0) { // Move
		if(move(posTmpX, posTmpY) == 0)
			empilha(&m->pil, (OPERANDO){BOOL, false}); //empilhaNoRobo false
		else
			empilha(&m->pil, (OPERANDO){BOOL, true}); //empilhaNoRobo true
	}
    else if(op == 1) { // Ataque
		if(ataque(posTmpX, posTmpY))
			empilha(&m->pil, (OPERANDO){BOOL, true}); //empilhaNoRobo true
		else
			empilha(&m->pil, (OPERANDO){BOOL, false}); //empilhaNoRobo false

    }
    else if(op == 3) { // Coletar
		if(coleta(posTmpX, posTmpY)) {
			robos[timeAtual][roboAtual]->crist++;
			empilha(&m->pil, (OPERANDO){BOOL, true}); //empilhaNoRobo true
		}
      else
        empilha(&m->pil, (OPERANDO){BOOL, false}); //empilhaNoRobo false
    }
    else if(op == 4) { // Depositar
      if(robos[timeAtual][roboAtual]->crist) {
        if(arena[posTmpX][posTmpY]->base){
          pontosTotais[arena[posTmpX][posTmpY]->base -1]++;
          cristaisRestantes--;
          if(cristaisRestantes == 0)
            Fim();
        }
        else {
           arena[posTmpX][posTmpY]->nCristais++;
        }
        robos[timeAtual][robo]->crist--;
		empilha(&m->pil, (OPERANDO){BOOL, true}); //empilhaNoRobo true
      }
      else
        empilha(&m->pil, (OPERANDO){BOOL, false}); //empilhaNoRobo false
    }
  }
  TempoDeCadaRobo[timeAtual][roboAtual]++;
  Atualiza();
}

/*-------------------------------------------------------------------------------------*/
//                                                                                     //
//                                        Fim                                          //
//                                                                                     //
/*-------------------------------------------------------------------------------------*/
void Fim () {
  clock_t end = clock();
  double currentTime = (double) (end - begin)/ CLOCKS_PER_SEC;

  if(cristaisRestantes != 0){
	printf("Esse jogo foi longe demais...\n");
  }
  int ganhador = 0;
  for(int i = 1; i < pontosTotais.length; i++) {
	if(pontosTotais[i-1] < pontosTotais[i]) {
		ganhador = i;
	}
  }
  int empate = 0;
  for(int i = 1; i < pontosTotais.length; i++) {
	if(pontosTotais[i] == pontosTotais[ganhador] && empate == 0 && ganhador != i) {
		printf("Empate! Times vencedores:\n");
		printf("Time %d;", (i+1));
		printf("Time %d;", ganhador);
	}
	else if(empate == 1  && ganhador != i){
		printf("Time %d;", (i+1));
	}
  }
  if(empate != 0) {
	printf("Time ganhador: Time %d\n Parabéns!\n", ++ganhador);
  }
  else {
	printf("Parabéns à todos!\n");
	printf("Tempo total de jogo: %d\n", currentTime);
  }

}

/*-------------------------------------------------------------------------------------*/
//                                                                                     //
//                                      Atualiza                                       //
//                                                                                     //
/*-------------------------------------------------------------------------------------*/
void Atualiza (){

  // Verifica se o jogo não foi muito longe
  if(RodadaAtual < 500) {

  	// Verifica se o robo não chamou mais do que 5 vezes o sistema nessa rodada
  	if(TempoDeCadaRobo[timeAtual][roboAtual] > RodadaAtual*5){
  		// Se chamou, passa a vez
  		timeAtual++;
  		if(timeAtual == TotTimes -1) {
  			timeAtual = 0;
  			roboAtual++;
  			// Caso tenha acabado a rodada:
  			if(roboAtual == TotRobTime -1) {
  				roboAtual = 0;
  				RodadaAtual++;
  			}
  		}
  	}
    // Verifica se o robo morreu
    if(RobosAtivos[timeAtual][roboAtual]) {
      Atualiza();
    }
    // Caso ainda esteja vivo
    else{
      exec_maquina(robos[timeAtual][roboAtual]->m, 50);
    }
  }
  else {
	   Fim();
  }
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
	int temRobo = 0
    int time = 0;
    int qual = 0;
    for(int i = 0; i < robos.length; i++){
      for(int j = 0; i < robos[j].length; j++){
        if(robos[time][qual]->posx == posTmpX && robos[time][qual]->posy == posTmpY) {
          time = i;
          qual = j;
		  temRobo = 1;
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
static int move(int posTmpX, int posTmpY){

  // Tem alguem ai? -- possivelmente adicionar se tem base
  if(arena[posTmpX][posTmpY]->vazia != 0){
    return 0;
  }
  else {
    // Muda estado da arena
    arena[robos[timeAtual][roboAtual]->posx][robos[timeAtual][roboAtual]->posy]->vazia = 0;
    arena[posTmpX][posTmpY]->vazia = timeAtual + 1;
    // Muda estado do robo
    robos[timeAtual][roboAtual]->posx = posTmpX;
    robos[timeAtual][roboAtual]->posy = posTmpY;
	TempoDeCadaRobo[timeAtual][roboAtual] += arena[posTmpX][posTmpY]->terreno;
    return 1;
  }
}

/*-------------------------------------------------------------------------------------*/
//                                                                                     //
//                                      Cria Robo                                      //
//                                                                                     //
/*-------------------------------------------------------------------------------------*/
static void InsereExercito (int time, int posX, int posY, int qual) {
  robos[time-1][qual] = cria_maquina(instrucao[time-1][qual], posX, posY, 100, 0, time);
  RobosAtivos[time-1][qual] = 1;
  TempoDeCadaRobo[time-1][qual] = 0;
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

  destroi_maquina(robos[time][qual]);
  RobosAtivos[time-1][qual] = 0;
}

/*-------------------------------------------------------------------------------------*/
//                                                                                     //
//                                     Cria Arena                                      //
//                                                                                     //
/*-------------------------------------------------------------------------------------*/
static int CriaArena(int tamanho, int times, int cristais, int robosT){

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
    for(int i = 0; i < robosT; i++){
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
