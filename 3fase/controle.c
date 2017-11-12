#include <stdio.h>
#include "controle.h"

FILE *display;
FILE *arena;
Robot rb[2];

void anda(int ri) {
  Robot r = rb[ri];

  if (r.pi == 14) r.vi = -1;
  if (r.pi == 0)  r.vi =  1;

  if (r.pj == 14) r.vj = -1;
  if (r.pj == 0)  r.vj =  1;

  r.i = r.pi + r.vi;
  r.j = r.pj + r.vj;
  rb[ri] = r;
  mostra(ri);
}
void inicializaGraf()
{
  fclose(arena);
  //int t; 						/* tempo */
  // pipe direto para o programa apres
  display = popen("python apres arena.txt", "w");

}
void atualiza(int ri) {
  Robot r = rb[ri];
  r.pi = r.i;
  r.pj = r.j;
  rb[ri] = r;
}

/***********************
* A função desenha célula recebe os argumentos px, py e terreno (inteiro) de
* uma célula e mantem escrevendo esses valores no arquivo arena.txt
* o arena.txt está no formato do protocolo do apres e será enviado na entrada padrão
* do apres quando a função inicializaGraf for invocada.
***********************/
void desenhaCelula (int px, int py, int terreno)
{
  // se o arquivo arena.txt não existir
  if (arena == NULL)
    // então cria
    arena = fopen("arena.txt", "w");
  fprintf(arena, "d_cel %d %d %d\n", px, py, terreno);
}

// metodo de testes. precisamos definir como os robos serão desenhados e movidos
void desenhaRobo (int i, int j)
{
  rb[0].pi = 15;
  rb[0].pj = 15;
  rb[0].vi = 1;
  rb[0].vj = 1;
  fprintf(display, "rob rt.png");
  mostra(0);
}

// imprime no arquvivo display que vai imediatamente para o apres
void mostra(int ri) {
  fprintf(display, "%d %d %d %d %d\n",
		  ri, rb[ri].pi, rb[ri].pj, rb[ri].i, rb[ri].j);
  atualiza(ri);
}

void DesenhaRobo2 (int qual, int Xantigo, int Yantigo, int Xnovo, int Ynovo){
  fprintf(display, "%d %d %d %d %d\n", qual, Xantigo, Yantigo, Xnovo, Ynovo);
}

void NovoRoboDesenho (int time){
  if(time == 1)
    fprintf(display, "rob GILEAD_A.png\n");
  else
    fprintf(display, "rob GILEAD_B.png\n");
}

/* Programa simples para mostrar como controlar a arena */
/*int main() {
  int t; 						/* tempo */
  // pipe direto para o programa apres
  /*display = popen("python apres", "w");


  rb[0].pi =  6;
  rb[0].pj = 14;
  rb[0].vi = -1;
  rb[0].vj =  1;

  rb[1].pi = 10;
  rb[1].pj = 11;
  rb[1].vi =  1;
  rb[1].vj = -1;

  if (display == NULL) {
	fprintf(stderr,"Não encontrei o programa de exibição\n");
	return 1;
  }

  /* cria dois robôs
  fprintf(display, "rob GILEAD_A.png\nrob GILEAD_B.png\n");


  for (t=0; t < 10; t++) {
	anda(0);
	anda(1);
	mostra(0);
	mostra(1);
	fflush(display);
  }
   pclose(display)
}*/
