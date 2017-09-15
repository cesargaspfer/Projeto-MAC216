#include <stdio.h>
#include"maq.h"
INSTR prog[] = {
 {INSTR, prog[]},
 {{PUSH,, 1},},
 {{STO,, 1},},
 {{PUSH,, 10},},
 {(), 0},
 {{RCL,, 1},},
 {{STO,, 0},},
 {{DUP,, 0},},
 {{PRN,, 0},},
 {{PUSH,, 1},},
 {{DUP,, 0},},
 {{PUSH,, 0},},
 {{JIF,, LOOP},},
 {};, 0},
};

int main(int ac, char **av) {
	Maquina *maq = cria_maquina(prog);
	exec_maquina(maq, 100);
	destroi_maquina(maq);
	return 0;
}
