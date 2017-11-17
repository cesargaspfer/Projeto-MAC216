#include <stdio.h>
#include "maq.h"
INSTR prog[] = {
{{MOV,, NONE, {ACAO,, {EAST}},}},
{{MOV,, NONE, {ACAO,, {NORTHEAST}},}},
{{MOV,, NONE, {ACAO,, {WEST}},}},
{{MOV,, NONE, {ACAO,, {SOUTHWEST}},}},
{{MOV,, NONE, {ACAO,, {CURRENT}},}},
{{PRN,, NONE, {NONE,, {0}},}},
{{END,, NONE, {NONE,, {0}}}},
};

int main(int ac, char **av) {
CriaArena(20, 2, 10, 5);
	Maquina *maq = cria_maquina(prog, 5, 5, 100, 0, 1);
	exec_maquina(maq, 1000);
	destroi_maquina(maq);
	return 0;
}
