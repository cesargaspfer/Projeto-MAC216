#include <stdio.h>
#include <stdlib.h>
#include "maq.h"

/* #define DEBUG */

#ifdef DEBUG
#  define D(X) X
char *CODES[] = {
  "PUSH",
  "POP",
  "DUP",
  "ADD",
  "SUB",
  "MUL",
  "DIV",
  "JMP",
  "JIT",
  "JIF",
  "CALL",
  "RET",
  "STS",
  "RCS",
  "EQ",
  "GT",
  "GE",
  "LT",
  "LE",
  "NE",
  "STO",
  "RCL",
  "END",
  "PRN",
  "STL",
  "RCE",
  "ALC",
  "FRE"
};
#else
#  define D(X)
#endif

static void Erro(char *msg) {
  fprintf(stderr, "%s\n", msg);
}

static void Fatal(char *msg, int cod) {
  Erro(msg);
  exit(cod);
}

Maquina *cria_maquina(INSTR *p) {
  Maquina *m = (Maquina*)malloc(sizeof(Maquina));
  if (!m) Fatal("Memória insuficiente",4);
  m->ip = 0;
  m->prog = p;
  return m;
}

void destroi_maquina(Maquina *m) {
  free(m);
}

/* Alguns macros para facilitar a leitura do código */
#define ip (m->ip)
#define pil (&m->pil)
#define exec (&m->exec)
#define prg (m->prog)

void exec_maquina(Maquina *m, int n) {
  int i;
  int al = 0;
  int temp = 0;
  for (i = 0; i < n; i++) {
	   OpCode   opc = prg[ip].instr;
	    OPERANDO arg = prg[ip].op;

	D(printf("%3d: %-4.4s %d\n     ", ip, CODES[opc], arg));

	switch (opc) {
	  OPERANDO tmp;
	case PUSH:
	  empilha(pil, arg);
	  break;
	case POP:
	  desempilha(pil);
	  break;
	case DUP:
	  tmp = desempilha(pil);
	  empilha(pil, tmp);
	  empilha(pil, tmp);
	  break;
	case ADD:
	  empilha(pil, desempilha(pil)+desempilha(pil));
	  break;
	case SUB:
	  tmp = desempilha(pil);
	  empilha(pil, desempilha(pil)-tmp);
	  break;
	case MUL:
	  empilha(pil, desempilha(pil)*desempilha(pil));
	  break;
	case DIV:
	  tmp = desempilha(pil);
	  empilha(pil, desempilha(pil)/tmp);
	  break;
	case JMP:
	  ip = arg;
	  continue;
	case JIT:
	  if (desempilha(pil) != 0) {
		ip = arg;
		continue;
	  }
	  break;
	case JIF:
	  if (desempilha(pil) == 0) {
		ip = arg;
		continue;
	  }
	  break;
	case CALL:
	  empilha(exec, ip);
	  ip = arg;
	  continue;
	case RET:
	  ip = desempilha(exec);
	  break;
	case EQ:
	  if (desempilha(pil) == desempilha(pil))
		empilha(pil, 1);
	  else
		empilha(pil, 0);
	  break;
	case GT:
	  if (desempilha(pil) < desempilha(pil))
		empilha(pil, 1);
	  else
		empilha(pil, 0);
	  break;
	case GE:
	  if (desempilha(pil) <= desempilha(pil))
		empilha(pil, 1);
	  else
		empilha(pil, 0);
	  break;
	case LT:
	  if (desempilha(pil) > desempilha(pil))
		empilha(pil, 1);
	  else
		empilha(pil, 0);
	  break;
	case LE:
	  if (desempilha(pil) >= desempilha(pil))
		empilha(pil, 1);
	  else
		empilha(pil, 0);
	  break;
	case NE:
	  if (desempilha(pil) != desempilha(pil))
		empilha(pil, 1);
	  else
		empilha(pil, 0);
	  break;
	case STO:
	  m->Mem[arg] = desempilha(pil);
	  break;
  case RCL:
	  empilha(pil,m->Mem[arg]);
	  break;
	case END:
	  return;
	case PRN:
	  printf("%d\n", desempilha(pil));
	  break;
  case STL:
    temp = desempilha(pil); //
    empilha(pil, temp);
    al = 0;
    while (al != arg +1){
      empilha(pil, desempilha(exec));
      al++;
    }
    empilha(exec, temp);
    temp = desempilha(pil);
    al = 0;
    while(al != arg){
      empilha(exec, desempilha(pil));
      al++;
    }
    break;
  case RCE: //Copia na pilha de dados o valor dado por args da pilha de exec
    al = 0;
    while(al != arg){
      empilha(pil, desempilha(exec));
      al++;
    }
    temp = desempilha(exec); //
    empilha(exec, temp);
    al = 0;
    while (al != arg){
      empilha(exec, desempilha(pil));
      al++;
    }
    empilha(pil, temp);
    break;
  case ALC:
    al = 0;
    while (al != arg)
    {
      empilha(exec, 0);
        al++;
    }
    empilha(exec, arg);
    break;
  case FRE:
    al = desempilha(exec);
    while (al != 0)
    {
      temp = desempilha(exec);
      al--;
    }
    break;
	}
	D(imprime(pil,5));
	D(puts("\n"));

	ip++;
  }
}
/*
Alteracoes
  1- Adicionado o novo registrador de base (rbp)
  2- Adicionado o STL
  3- Adicionado o RCL (Temporario)
  4- Alteracao no RET (verificar)

Notas:
  1- O RCL esta duplicado no enunciado, esperar resp do prof no paca
  2- O RET, no enunciado, fala que empilha um valor, mas nao eh pedido para alterarmos isso
  3- Verificar como tem que "acertar" o registrador de base (rbp) no RET


*/
