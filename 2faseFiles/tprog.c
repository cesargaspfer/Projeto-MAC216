#include <stdio.h>
#include "maq.h"
INSTR prog[] = {
{INF, NONE, {CURRENT}},
{PRN, NONE, {0}},
{MOV, ACAO, {SOUTH}},
{PRN, NONE, {0}},
{POP, NONE, {0}},
{PRN, NONE, {0}},
{MOV, ACAO, {SOUTH}},
{POP, NONE, {0}},
{PRN, NONE, {0}},
{MOV, ACAO, {SOUTH}},
{POP, NONE, {0}},
{PRN, NONE, {0}},
{MOV, ACAO, {NORTHEAST}},
{POP, NONE, {0}},
{PRN, NONE, {0}},
{MOV, ACAO, {NORTHEAST}},
{POP, NONE, {0}},
{PRN, NONE, {0}},
{MOV, ACAO, {SOUTHWEST}},
{POP, NONE, {0}},
{PRN, NONE, {0}},
{MOV, ACAO, {SOUTHWEST}},
{POP, NONE, {0}},
{PRN, NONE, {0}},
{END, NONE, {0}},
};

int main(int ac, char **av) {
CriaArena(20, 2, 10, 5);
	Maquina *maq = cria_maquina(prog, 0, 0, 100, 0, 1);
	exec_maquina(maq, 1000);
	destroi_maquina(maq);
	return 0;
}
