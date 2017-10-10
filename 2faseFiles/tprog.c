#include <stdio.h>
#include "maq.h"
INSTR prog[] = {
{PUSH , NUM  ,{ 10 } },
{POP ,NONE,{0} },
{PUSH , NUM  ,{ 30 } },
{PUSH , NUM  ,{ 20 } },
{PRN ,NONE,{0} },
{MOV , ACAO ,{ 7 } },
{ATK , ACAO ,{ 5 } },
{END ,NONE,{0} }
};
int sizes[] = { 1, 1, 1, 1, 1, 1, 1, 1 };

int main(int ac, char **av) {
	Maquina *maq = cria_maquina(prog);
	exec_maquina(maq, 1000);
	destroi_maquina(maq);
	return 0;
}
