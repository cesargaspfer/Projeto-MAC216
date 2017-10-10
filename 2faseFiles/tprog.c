#include <stdio.h>
#include "maq.h"
INSTR prog[] = {
{PUSH is string this: 10
, NUM  } },
{POP ,NONE,{0} },
{DUP ,NONE,{0} },
{ADD ,NONE,{0} },
{SUB ,NONE,{0} },
{MUL ,NONE,{0} },
{DIV ,NONE,{0} },
{JMP is string this: 1
, NUM  } },
{JIT ,NONE,{0} },
{JIF ,NONE,{0} },
{CALL ,NONE,{0} },
{RET ,NONE,{0} },
{STS ,NONE,{0} },
{RCS ,NONE,{0} },
{EQ ,NONE,{0} },
{GT ,NONE,{0} },
{GE ,NONE,{0} },
{LT ,NONE,{0} },
{LE ,NONE,{0} },
{NE ,NONE,{0} },
{STO ,NONE,{0} },
{RCL ,NONE,{0} },
{END ,NONE,{0} },
{PRN ,NONE,{0} },
{STL ,NONE,{0} },
{RCE ,NONE,{0} },
{ALC ,NONE,{0} },
{FRE ,NONE,{0} },
{ATR ,NONE,{0} }
};
int sizes[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

int main(int ac, char **av) {
	Maquina *maq = cria_maquina(prog);
	exec_maquina(maq, 1000);
	destroi_maquina(maq);
	return 0;
}
