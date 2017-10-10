#include <stdio.h>
#include "maq.h"
INSTR prog[] = {
{PUSH,  NUM, { 10 } },
{PUSH,  CELL, { 3, 4 } },
{PUSH,  VAR, { 88, 90, 78 } },
{PUSH,  CELL, { 10, 20, 3, 123 } },
{END,  NONE,{0} }
};
int sizes[] = { 1, 2, 3, 4, 1 };

int main(int ac, char **av) {
	Maquina *maq = cria_maquina(prog, sizes);
	exec_maquina(maq, 1000);
	destroi_maquina(maq);
	return 0;
}
