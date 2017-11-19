#include <stdio.h>
#include "controle.h"

FILE *display;
Robot rb[10];

// termina de escrever o txt da arena e começa a mostrá-la e receber comandos
// pelo protocolo estabelecido na comunicação entre controle.c e apres
void inicializaGraf()
{
  // pipe direto para o programa apres
  display = popen("python apres", "w");
}

// substitui (oi,oj) por (di,dj). Ou seja, as antigas coordenadas de destino
// do robô agora são as de origem, pois ele foi para a célula (di,dj)
void atualiza(int index) {
  Robot r = rb[index];
  r.oi = r.di;
  r.oj = r.dj;
  rb[index] = r;
}

/***********************
* A função desenha célula recebe os argumentos px, py e terreno (inteiro) de
* uma célula e envia essas informações para o programa apres
***********************/
void desenhaCelula (int px, int py, int terreno)
{
  // envia para apres a instrução d_cel, as coordenadas (px,py) e o tipo de terreno
  fprintf(display, "d_cel %d %d %d\n", px, py, terreno);
  fflush(display);
}

void desenhaBase (int x, int y, int exercito)
{
  fprintf(display, "base flag%d.png %d %d\n", exercito, x, y);
  fflush(display);
}

void desenhaCristal (int x, int y, int n)
{
  fprintf(display, "cristal %d %d %d\n", x, y, n);
  fflush(display);
}

void desenhaArma (int x, int y)
{
  fprintf(display, "gun %d %d\n", x, y);
  fflush(display);
}

/*
Inicializa a sprite (a imagem) do robô no programa apres, que é responsável por
exibir a arena. O que esse método faz é simplesmente mandar uma instrução ao
programa apres que faça a imagem do robô ser adicionada na lista "robs" do programa
em python. Depois, chama o método mostra para que o robô seja movido para a posição
i j dada como parâmetro
*/
void desenhaRobo (int index, int exercito, int i, int j)
{
  if (exercito == 0)
    fprintf(display, "rob ra.png\n");
  else
    fprintf(display, "rob rb.png\n");
  fflush(display);
  rb[index].exercito = exercito;
  // o robô começa fora da arena
  rb[index].oi = -1;
  rb[index].oj = -1;
  // o robô tem sua célula de destino definida pelos parâmetros i,j
  rb[index].di = i;
  rb[index].dj = j;
  // a função 'mostra' enviará para a interface visual a instrução para "Movimentar"
  // o novo robô da posição (-1,-1) para a posição (i,j)
  mostra(index);
}

void removeItem(int x, int y)
{
  fprintf(display, "remitem %d %d\n", x, y);
  fflush(display);
}

void mostraAtaque()
{
  fprintf(display, "atk\n");
  fflush(display);
}

// mostra na arena o robô em index
void mostra(int index) {
  // display: ri oi oj di dj (o: origem) (d: destino) (i,j): coordenadas
  fprintf(display, "%d %d %d %d %d\n",
		  index, rb[index].oi, rb[index].oj, rb[index].di, rb[index].dj);
  fflush(display);
  // substitui (oi,oj) por (di,dj). Ou seja, as antigas coordenadas de destino
  // do robô agora são as de origem, pois ele foi para a célula (di,dj)
  atualiza(index);
}

void acabaDesenho () {
  pclose(display);
}

// move o robô no index dado para a posição (di, dj)
void moveRobo (int index, int di, int dj){
  // recupera o robô em index
  Robot r = rb[index];
  // muda as coordenadas de seu destino
  r.di = di;
  r.dj = dj;
  rb[index] = r;
  // envia para a interface visual a instrução para movê-lo
  mostra(index);
}
