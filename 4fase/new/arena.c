#include<stdio.h>
#include<stdlib.h>
#include "arena.h"
#include "maq.h"
#include<time.h>
#include "util.h"
#include "controle.h"

// Variaveis do jogo em si (Condicoes iniciais do jogo)

	static int TotTimes = 2;      // Total de times na partida
	static int TotRobTime = 5;    // Total de robos por partida
	static Celula arena[15][15];  // A arena em sí (o campo de batalha)
	Maquina *robos[2][5];   // Vetor que contem os robos ([times][robos])

// Variáveis da arena
	static int cristaisRestantes;   // Cristais restantes na arena

// Variaveis do sistema

	// Variavies de execucao (indices para as matrizes)
	static int timeAtual = 0;   // Qual time o robo que esta sendo executado pertence
	static int roboAtual = 0;   // Do time, qual robo esta sendo executado

	// Variaveis de controle
	static int pontosTotais [2];    // Pontos totais de cada time,
	static int RobosAtivos[2][5];   // Vetor que guarda se o robo esta ativo ou nao.
                                  // linhas: times; colunas: robôs
	static int fimDoJogo = 0;       // Guarda a informacao se o jogo ja acabou

	// Variavel de tempo
	clock_t begin; // "relogio" do começo
  clock_t end; // "relogio" do fim
  double currentTime = 0.0; // tempo total de jogo

  char *nomesArquivo[2][5] = {
    {"robo00", "robo01", "robo02", "robo03", "robo04"},
    {"robo10", "robo11", "robo12", "robo13", "robo14"},
  };

// Funcao main, a qual inicializa as variaveis e o jogo
int main () {
  // Cria a arena, com os argumentos:
  //	-Tamanho da arena,
  //	-Total de times,
  // 	-Total de cristais,
  // 	-Total de robos por time
  CriaArena(15, TotTimes, 20, TotRobTime, 2);
  // espera a música terminar
  waitFor(6);
  // Começa jogo!!
  Atualiza();
}
/*-------------------------------------------------------------------------------------*/
//                                                                                     //
//                                       Sistema                                       //
//                                                                                     //
/*-------------------------------------------------------------------------------------*/

/*************************************************
* As op's vão ser implementadas da seguinte maneira,
* MOV = movimenta
* ATK = ataca
* INF = informação
* CLT = coleta
* DEP = deposita
*
*************************************************/
// Recebe a operacao (acao) que ira executar, a direcao e de qual maquina esta solicitando
void Sistema(int op, int dir, Maquina *m) {
  // Ajusta o movimento da matriz hexagonal para a matriz quadrada
  int movX = 0; // Celula na direcao X a avançar
  int movY = 0; // Celula na direcao Y a avançar
  // Movimentar o robô de acordo com o argumento dado em Dir
  if(dir == EAST)
    movY = 1;
  else if(dir == WEST)
    movY = -1;
  else if (dir == CURRENT){
    movX = 0;
    movY = 0;
  }
  else if(m->posx%2 == 0){
    if(dir == NORTHEAST){
      movX = -1;
    }
    else if(dir == SOUTHEAST){
      movX = 1;
    }
    else if(dir == SOUTHWEST){
      movX = 1;
      movY = -1;
    }
    else if(dir == NORTHWEST){
      movX = -1;
      movY = -1;
    }
  }
  else{
    if(dir == NORTHEAST){
      movX = -1;
      movY = 1;
    } else if(dir == SOUTHEAST){
      movX = 1;
      movY = 1;
    } else if(dir == SOUTHWEST){
      movX = 1;
    } else if(dir == NORTHWEST){
      movX = -1;
    }
  }


  // Futura posição do robô na arena
  int x = m->posx + movX;
  int y = m->posy + movY;
  //Fora do mapa?
  if(x < 0 || y < 0 || x > 14 || y > 14) {
    empilha(&m->pil, (OPERANDO){BOOL, false}); //Empilha, no robo, false
    printf("Direção inválida: %d %d\n", x, y);
  }
  else{
    if(op == INF) { // Se pedir infos
      OPERANDO o;
      o.t = CELL;
      o.Valor.c = arena[x][y];
      
      empilha(&m->pil, o); // Empilha, no robo, a Celula
    }
    else if(op == MOV) { // Move
  		if(move(x, y, m) == 0)
      {
        empilha(&m->pil, (OPERANDO){BOOL, false}); //Empliha, no robo, false
        printf("Não conseguiu se mover para a posição (%d,%d)\n", x, y);
      }
  		else
      {
        printf("Conseguiu se mover para a posição (%d,%d)\n", x, y);
        empilha(&m->pil, (OPERANDO){BOOL, true}); //Empliha, no robo, true
      }

  	}
    else if(op == ATK) { // Ataque
  		if(ataque(x, y, m))
      {
        printf("Atacou a posição (%d,%d)\n", x, y);
        empilha(&m->pil, (OPERANDO){BOOL, true}); //Empliha, no robo, true
      }
  		else
      {
        printf("Não conseguiu atacar a posição (%d,%d)\n", x, y);
        empilha(&m->pil, (OPERANDO){BOOL, false}); //Empliha, no robo, false
      }
    }
    else if(op == CLT) { // Coletar
		if(coleta(x, y, m)) {
      // indica ao robô que ele coletou um item
			empilha(&m->pil, (OPERANDO){BOOL, true});
      if (arena[x][y].coletavel == NENHUM){
        // retirar o desenho do cristal se não há mais nenhum cristal na célula
        removeItem(x,y);
        if (arena[x][y].vazia) {
          int time = -1;
          int qual = -1;
          for(int i = 0; i < TotTimes; i++){
            if (time != -1 && qual != -1)
              break;
            for(int j = 0; i < TotRobTime; j++){
              if((robos[i][j])->posx == x && robos[i][j]->posy == y) {
                time = i;
                qual = j;
              }
              if (time != -1 && qual != -1)
                break;
            }
          }
          moveRobo(qual + TotRobTime*time, x, y);
        }
      }
      printf("Coletou da posição (%d,%d)\n", x, y);
		}
      else
      {
        printf("Não conseguiu coletar da posição (%d,%d)\n", x, y);
        // indica para o robô que ele não pode coletar um item (porque não tinha nada na célula)
        empilha(&m->pil, (OPERANDO){BOOL, false});
      }
    }
    else if(op == DEP) { // Depositar
      // Se o robo tiver cristais
      if(m->crist) {
        // Se tiver uma base a celula que ele quer depositar
        if(arena[x][y].base){
          printf("Depositou na posição (%d,%d)\n", x, y);
          // O time da base que foi depositado um cristal ganha um ponto
          pontosTotais[arena[x][y].base -1]++;
          cristaisRestantes--;
          // Caso nao tenha sobrado nenhum cristal mais no jogo
          if(cristaisRestantes == 0)
          {
            Fim();
          }
        }
        // Se nao for base
        else {
          printf("Depositou na posição (%d,%d)\n", x, y);
           arena[x][y].nCristais++;
        }
        // Remove um cristal do robo
        m->crist--;
        desenhaCristal(1, x, y);
		    empilha(&m->pil, (OPERANDO){BOOL, true}); //Empliha, no robo, true
      }
      // Caso ele nao tenha cristais
      else
      {
        printf("Não depositou na posição (%d,%d)\n", x, y);
        empilha(&m->pil, (OPERANDO){BOOL, false}); //Empliha, no robo, false
      }

    }
  }
  waitFor(1);
}

/*-------------------------------------------------------------------------------------*/
//                                                                                     //
//                                        Fim                                          //
//                                                                                     //
/*-------------------------------------------------------------------------------------*/
void Fim () {
  // "Para o relogio"
  end = clock();
  currentTime = (double) (end - begin)/ CLOCKS_PER_SEC;
  printf("\n\n");
  // Caso nao foram coletados todos os cristais, eh avisado
  // que o jogo nao foi ate o fim (ou seja, foi interrompido)
  if(cristaisRestantes != 0){
	  printf("Esse jogo foi longe demais...\n");
    //notificaFim(-1);
  }

  // Procura o ganhador (quem fez mais pontos)
  int ganhador = 0;
  // calcular o tamanho do array pontosTotais
  int len = sizeof(pontosTotais) / sizeof(pontosTotais[0]);

  for(int i = 1; i < TotTimes; i++) {
  	if(pontosTotais[i-1] < pontosTotais[i]) {
  		ganhador = i;
  	}
  }
  // Verifica se houve empate, se houve, avisa que empatou e quem ganhou
  int empate = 0;
  for(int i = 0; i < len; i++) {
  	if(pontosTotais[i] == pontosTotais[ganhador] && empate == 0 && ganhador != i) {
      printf("Empate!\n");
      //notificaFim(++ganhador);
  	}
  	else if(empate == 1  && ganhador != i){
        printf("Time %d; ", (i+1));
        //notificaFim(-1);
  	}
  }
  // Caso nao houve empate, avisa o ganhador
  if(empate != 0) {
    printf("Time ganhador: Time %d\n Parabéns!\n", ++ganhador);
     //notificaFim(++ganhador);
  }
  // Da parabens a todos e diz quanto tempo levou essa partida
  else {
    printf("\nParabéns à todos!\n");
    //notificaFim(-1);
  }
  printf("Tempo total de jogo: %f\n", currentTime);
}

/*-------------------------------------------------------------------------------------*/
//                                                                                     //
//                                      Atualiza                                       //
//                                                                                     //
/*-------------------------------------------------------------------------------------*/
void Atualiza (){

  int RodadaAtual = 0; // Qual rodada estamos
  // Enquanto estiver abaixo da rodada 500:
  while(RodadaAtual < 500) {
    // Se acabou o jogo, pare com os 3 lacos
    if(fimDoJogo){
      break;
    }
	// A cada robo dentro do time
  	for(int i = 0; i < TotRobTime; i++){
  		// A cada time
  		for(int j = 0; j < TotTimes; j++){
  			// Muda o Time atual que está executando
  			timeAtual = j;
        // Muda qual robo dentro do time está executando
        roboAtual = i;
  			// Se o robo esta vivo:

  			if(RobosAtivos[timeAtual][roboAtual] == 1) {
  				// Executa 50 instrucoes
          if(robos[timeAtual][roboAtual]->energia){
            robos[timeAtual][roboAtual]->energia--;
          }
          else{
            exec_maquina(robos[timeAtual][roboAtual], 50);
          }

  			}
      }
  	}
    // "Proxima rodada"
    RodadaAtual++;
  	// Caso seja a ultima rodada, acaba com o jogo
  	if(RodadaAtual == 500){
  		Fim();
  	}
  }
}

/*-------------------------------------------------------------------------------------*/
//                                                                                     //
//                                       Coleta                                        //
//                                                                                     //
/*-------------------------------------------------------------------------------------*/
int coleta(int x, int y, Maquina *m){
  // Caso existam coletáveis na célula em (x, y):
  if(arena[x][y].coletavel != NENHUM){
    Coletavel c = arena[x][y].coletavel;
    switch (c)
    {
      case CRISTAL:
        // diminui o número de cristais na célula
        arena[x][y].nCristais--;
        // se foi pego o último cristal
        if (arena[x][y].nCristais == 0)
          // não há mais nenhum coletável
          arena[x][y].coletavel = NENHUM;
        // o robô está carregando mais um executável
        m->crist++;
        break;
      case ARMA:
        arena[x][y].coletavel = NENHUM;
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
int ataque(int x, int y, Maquina *m){
  // Caso tenha robo para atacar na posicao desejada:
  if(arena[x][y].vazia != 0){
    m->energia += 3;
    // Procura o robo nessa posicao
    int time = -1;
    int qual = -1;
    for(int i = 0; i < TotTimes; i++){
      if (time != -1 && qual != -1)
        break;
      for(int j = 0; i < TotRobTime; j++){
        if(robos[i][j]->posx == x && robos[i][j]->posy == y) {
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
    robos[time][qual]->vida -= m->dano;
    printf("Saúde do robô atacado: %d\n", robos[time][qual]->vida);
    // Caso acabe a vida desse robo, ele será destruido
    if(robos[time][qual]->vida <= 0){
      // remove o robô de seu exército
      destroiRobo(x, y, time, qual);
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
int move(int x, int y, Maquina *m){

  // Tem alguem ai? - Verifica se ja tem um robo na celula desejada
  if(arena[x][y].vazia != 0){

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
    arena[ox][oy].vazia = 0;
    // Marca a celula que o robo esta indo para nao vazia, indicando o seu time
    arena[x][y].vazia =  timeAtual + 1;
    // Muda estado do robo
    // Muda sua posicao
    moveRobo(roboAtual + (timeAtual)*5, x, y);

    m->posx = x;
    m->posy = y;
    m->energia = arena[x][y].terreno;
    // se na célula pela qual o robô passou havia algum item que não foi coletado
    if (arena[ox][oy].coletavel != NENHUM)
    {
      // redesenha o item
      if (arena[ox][oy].coletavel == CRISTAL)
        desenhaCristal(1, ox, oy);
      else if (arena[ox][oy].coletavel == ARMA)
        desenhaArma(ox, oy);
        //Se havia uma base
    } else if (arena[ox][oy].base != 0)
    {
      // desenha a base novamente
      desenhaBase(ox,oy,arena[ox][oy].base);
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
static void inicializaRobo (int index, int time, int posX, int posY) {

  printf("inicializou robô %d no time %d na posição (%d , %d)\n", index, time, posX, posY);
  // Cria o robo com a função cria_maquina
  robos[time][index] = cria_maquina(nomesArquivo[time][index], posX, posY, time);
  // Marca como "ativo" esse robo no vetor de robos ativos
  RobosAtivos[time][index] = 1;
}

/*-------------------------------------------------------------------------------------*/
//                                                                                     //
//                                    Destroi Robo                                     //
//                                                                                     //
/*-------------------------------------------------------------------------------------*/
void destroiRobo(int posX, int posY, int time, int qual) {

  //Caso não foi informado de qual time o robo pertence
  if(time == -1){
    // Procura o robo a destruir, pela sua posição
    time = 0;
    qual = 0;
    for(int i = 0; i < TotTimes; i++){
      for(int j = 0; i < TotRobTime; j++){
        if(robos[time][qual]->posx == posX && robos[time][qual]->posy == posY) {
          time = i;
          qual = j;
          break;
        }
      }
    }
  }
  int terreno = arena[robos[time][qual]->posx][robos[time][qual]->posy].terreno;
  desenhaCelula(robos[time][qual]->posx, robos[time][qual]->posy, terreno);
  if(arena[robos[time][qual]->posx][robos[time][qual]->posy].nCristais){
    desenhaCristal(1, robos[time][qual]->posx, robos[time][qual]->posy);
  }
  // "Destroi" o robo no vetor
  robos[time][qual]->posx = -1;
  robos[time][qual]->posy = -1;
  robos[time][qual]->vida = 0;

  // Caso ele tenha cristais, ele os derruba (para uma posicao aleatoria, mas perto dele)
  for(int i = 0; i < robos[time][qual]->crist; i++){
    int localX = rand() % 3 -1;
    int localY = rand() % 3 -1;
    int cairX = posX + localX;
    int cairY = posY + localY;

    //Verifica se nao caiu dentro da arena, caso nao tenha caido, bota na extremidade da arena
         if (cairX < 0)  cairX = 0;
    else if (cairX > 19) cairX = 19;
         if (cairY < 0)  cairY = 0;
    else if (cairY > 19) cairY = 19;
    arena[posX][posY].nCristais++;
    arena[posX][posY].coletavel = CRISTAL;
    // desenha 1 cristal na célula (x,y)
    desenhaCristal(1, posX, posY);
  }
  robos[time][qual]->crist = 0;

  // Marca como "inativo" esse robo no vetor de robos ativos
  RobosAtivos[time][qual] = 0;
}

/*-------------------------------------------------------------------------------------*/
//                                                                                     //
//                                     Cria Arena                                      //
//                                                                                     //
/*-------------------------------------------------------------------------------------*/
int CriaArena(int tamanho, int times, int cristais, int robosT, int armas){
  // Inicialização dos vetores de cristais e dos pontos de cada time
  cristaisRestantes = cristais;

  for(int i = 0; i < times; i++){
    pontosTotais[i] = 0;
  }


  fimDoJogo = 0; // O jogo não terminou

   timeAtual = 0;
	 roboAtual = 0;


  time_t t;
  srand((unsigned) time(&t));
  // Gera a arena, definindo aleatoriamente seus terrenos
  for(int i = 0; i < 15; i++){
   for(int j = 0; j < 15; j++){
     // define um dos cinco terrenos ( de 0 a 4)
     arena[i][j].terreno = rand() % 5;
     // inicializa sempre vazia
     arena[i][j].vazia = 0;
     // inicializa sem nenhum cristal, pois é preciso definir se a celula em questão é base ou não
     arena[i][j].nCristais = 0;
     // por enquanto, não a cristais em nenhuma das células
     arena[i][j].coletavel = NENHUM;
   }
 }

 //Inicializa a construção da visualização
 inicializaGraf();
 // envia a arena para a visualização, sem robôs ou coletáveis ainda
 for (int i = 0; i < tamanho; i++)
 {
   for (int j = 0; j < tamanho; j++){
     Celula c = arena[i][j];
     desenhaCelula(i, j, c.terreno);
   }
 }

 // define as duas bases de cada time e as desenha
 for (int i = 1; i <= times; i++)
 {
   // sorteia a posição da base (a primeira na metade superior da arena, a segunda na metade superior)
   int x = rand()%(7) + 7*(i-1);
   int y = rand()%(7) + 7*(i-1);
   if (arena[x][y].base)
   {
    i--;
    continue;
   }

   // define como verdadeiro na struct que a célula é uma base
   arena[x][y].base = i;
   // define o "tipo de terreno" como BASE
   arena[x][y].terreno = BASE;
   desenhaBase(x,y,i);
 }


  // Distribuir cristais pela arena, de modo a não colocar um cristal onde já tem um robô ou
  // uma base e os desenha
  for (int i = 0; i < cristais; i++)
  {

      int x = rand()%14;
      int y = rand()%14;
      if (arena[x][y].vazia != 0 || arena[x][y].base != 0)
        i--;
      else
      {
        arena[x][y].nCristais++;
        arena[x][y].coletavel = CRISTAL;
        // desenha 1 cristal na célula (x,y)
        desenhaCristal(1, x, y);
      }
  }

  // Distribuir duas armas na arena em células não ocupadas por bases ou cristais
  for (int i = 0; i < armas;)
  {
    int x = rand()%14;
    int y = rand()%14;
    if (arena[x][y].vazia == 0 && arena[x][y].coletavel == NENHUM)
    {
      // define o coletável da célula(x,y) como ARMA
      arena[x][y].coletavel = ARMA;
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
      if(arena[localX][localY].vazia || arena[localX][localY].base || arena[localX][localY].coletavel != NENHUM){
        i--;
      }
      // Se estiver vazia:
      else{
        // marca a arena como ocupada (vazia = 0, sennao representa qual time esta ai)
        arena[localX][localY].vazia = j;
        // desenha o robô "index" do exército j
        desenhaRobo(index, j-1, localX, localY);
        // coloca o robô i no time j
        inicializaRobo(i, j-1, localX, localY);
        index++;
      }
    }
  }
  // "Inicializa o relogio"
  begin = clock();

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

// Funcao que espera um certo tempo
void waitFor (unsigned int secs) {
    unsigned int retTime = time(0) + secs;
    while (time(0) < retTime);
}
