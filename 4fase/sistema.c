#include<stdio.h>
#include<stdlib.h>
#include "arena.h"
#include "arenaMaster.h"
#include "maq.h"
#include "tprog.h"
#include<time.h>
#include "util.h"
#include "controle.h"
#include "pilha.h"

// Variaveis do jogo em si (Condicoes iniciais do jogo)

	static int TotTimes = 2;      // Total de times na partida
	static int TotRobTime = 5;    // Total de robos por partida
  Arena ArenaSt;


// Variaveis do sistema

	// Variavies de execucao (indices para as matrizes)
	static int timeAtual = 0;   // Qual time o robo que esta sendo executado pertence
	static int roboAtual = 0;   // Do time, qual robo esta sendo executado

	// Variaveis de controle
	static int pontosTotais [2];    // Pontos totais de cada time,
                                  // linhas: times; colunas: robôs
	static int fimDoJogo = 0;       // Guarda a informacao se o jogo ja acabou

  static int cristaisRestantes = 20;
	// Variavel de tempo
	clock_t begin; // "relogio" do começo
  clock_t end; // "relogio" do fim
  double currentTime = 0.0; // tempo total de jogo


// Funcao main, a qual inicializa as variaveis e o jogo
int main () {
printf("!OI!");
  for(int i = 0; i < 2; i++)
    pontosTotais[i] = 0;

  fimDoJogo = 0;
  currentTime = 0;
  cristaisRestantes = 20;

  printf("!OI!");
  // Cria a arena, com os argumentos:
  //	-Tamanho da arena,
  //	-Total de times,
  // 	-Total de cristais,
  // 	-Total de robos por time
  ArenaSt = CriaArena(15, TotTimes, 20, TotRobTime, 2);
  // espera a música terminar
  waitFor(6);

  // "Inicializa o relogio"
  begin = clock();
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
      o.Valor.c = ArenaSt.arena[x][y];
      empilha(&m->pil, o); // Empilha, no robo, a Celula
    }
    else if(op == MOV) { // Move
  		if(move(x, y, m, &ArenaSt, timeAtual, roboAtual) == 0)
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
  		if(ataque(x, y, m, &ArenaSt))
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
		if(coleta(x, y, m, &ArenaSt)) {
      // indica ao robô que ele coletou um item
			empilha(&m->pil, (OPERANDO){BOOL, true});
      if (ArenaSt.arena[x][y].coletavel == NENHUM){
        // retirar o desenho do cristal se não há mais nenhum cristal na célula
        removeItem(x,y);
        if (ArenaSt.arena[x][y].vazia) {
          int time = -1;
          int qual = -1;
          for(int i = 0; i < TotTimes; i++){
            if (time != -1 && qual != -1)
              break;
            for(int j = 0; i < TotRobTime; j++){
              if(ArenaSt.robos[i][j]->posx == x && ArenaSt.robos[i][j]->posy == y) {
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
    // Depositar ficou implementado em sistema para facilitar a manipulacao dos pontos e do fim
    else if(op == DEP) { // Depositar
      // Se o robo tiver cristais
      if(m->crist) {
        // Se tiver uma base a celula que ele quer depositar
        if(ArenaSt.arena[x][y].base){
          printf("Depositou na posição (%d,%d)\n", x, y);
          // O time da base que foi depositado um cristal ganha um ponto
          pontosTotais[ArenaSt.arena[x][y].base -1]++;
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
           ArenaSt.arena[x][y].nCristais++;
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

  			if(ArenaSt.RobosAtivos[timeAtual][roboAtual] == 1) {
  				// Executa 50 instrucoes
          if(ArenaSt.robos[timeAtual][roboAtual]->energia){
            ArenaSt.robos[timeAtual][roboAtual]->energia--;
          }
          else
  				  exec_maquina(ArenaSt.robos[timeAtual][roboAtual], 50);
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



// Funcao que espera um certo tempo
void waitFor (unsigned int secs) {
    unsigned int retTime = time(0) + secs;
    while (time(0) < retTime);
}
