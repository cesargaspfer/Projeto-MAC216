#include <stdio.h>
#include "maq.h"
INSTR prog[] = {
{PUSH, NUM, {10}},
{POP, NONE, {0}},
{PUSH, NUM, {30}},
{PUSH, NUM, {20}},
{PRN, NONE, {0}},
{MOV, ACAO, {7}},
{ATK, ACAO, {5}},
{END, NONE, {0}},
};

INSTR[] getProg() { return prog; }
