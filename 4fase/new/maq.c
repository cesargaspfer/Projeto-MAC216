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
  "ATR",
  "SIS",
  "ENTRY",
  "LEAVE"
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
  m->pil.topo = 0;
  m->ib = 0;
  m->posx = x;
  m->posy = y;
  m->exercito = exercito;
  m->vida = 100;
  m->crist = 0;
  m->energia = 0;
  m->dano = 10;
  return m;
}

void destroi_maquina(Maquina *m) {
  free(m);
}

int new_frame(Maquina *m, int n) {
  int ibc = m->ib;
  if (ibc < MAXFRM-1) {
	   m->bp[++m->ib] = n+ibc;
	    return m->ib;
  }
  return -1;
}

int del_frame(Maquina *m) {
  if (m->ib > 0) return --m->ib;
  return -1;
}

/* Alguns macros para facilitar a leitura do código */
#define ip (m->ip)
#define pil (&m->pil)
#define exec (&m->exec)
#define prg (m->prog)

void exec_maquina(Maquina *m, int n) {
  int i;

  for (i = 0; i < n; i++) {
	OpCode   opc = prg[ip].instr;
	OPERANDO arg = prg[ip].op;

	D(printf("%3d: %-5.5s %d\n", ip, CODES[opc], arg.val.n));

	switch (opc) {
	  OPERANDO tmp;
	  OPERANDO a;
	  OPERANDO b;
	  OPERANDO res;

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
    // Desempilhar e somar os dois objetos no topo da pilha e
    // empilhar o resultado (OPERANDO result) sse eles forem do tipo NUM
    // Caso contrário, imprimir mensagem de erro e reempilhar os argumentos
    // retirados
    a = desempilha(pil);
    b = desempilha(pil);
    if (a.t == NUM && b.t == NUM)
    {
      res = (OPERANDO){NUM,a.Valor.n + b.Valor.n};
      empilha(pil, res);
    }
    else
    {
      empilha(pil, b);
      empilha(pil, a);
      Erro("Erro: ADD só definido para o tipo NUM");
    }
    break;
	case SUB:
    // Desempilhar e subtrair os dois objetos no topo da pilha e
    // empilhar o resultado (OPERANDO result) sse eles forem do tipo NUM
    // Caso contrário, imprimir mensagem de erro e reempilhar os argumentos
    // retirados
    a = desempilha(pil);
    b = desempilha(pil);
    if (a.t == NUM && b.t == NUM)
    {
      res = (OPERANDO){NUM,b.Valor.n - a.Valor.n};
      empilha(pil, res);
    }
    else
    {
      empilha(pil, b);
      empilha(pil, a);
      Erro("Erro: SUB só definido para o tipo NUM");
    }
    break;
	case MUL:
    // Desempilhar e multiplicar os dois objetos no topo da pilha e
    // empilhar o resultado (OPERANDO result) sse eles forem do tipo NUM
    // Caso contrário, imprimir mensagem de erro e reempilhar os argumentos
    // retirados
      a = desempilha(pil);
      b = desempilha(pil);
      if (a.t == NUM && b.t == NUM)
      {
        empilha(pil, (OPERANDO){NUM,a.Valor.n * b.Valor.n});
      }
      else
      {
        empilha(pil, b);
        empilha(pil, a);
        Erro("Erro: MUL só definido para o tipo NUM");
      }
      break;
	case DIV:
  // Desempilhar e dividir os dois objetos no topo da pilha e
  // empilhar o resultado (OPERANDO result) sse eles forem do tipo NUM
  // Caso contrário, imprimir mensagem de erro e reempilhar os argumentos
  // retirados
    a = desempilha(pil);
    b = desempilha(pil);
    if (a.t == NUM && b.t == NUM)
    {
      if (b.Valor.v == 0)
      {
        empilha(pil, b);
        empilha(pil, a);
        Erro("Erro: Divisão por zero.");
      }
      empilha(pil, (OPERANDO){NUM,a.Valor.n / b.Valor.n});
    }
    else
    {
      empilha(pil, b);
      empilha(pil, a);
      Erro("Erro: DIV só definido para o tipo NUM");
    }
    break;
	case JMP:
	  ip = arg.val.n;
	  continue;

	case JIT:
	  if (desempilha(pil).val.n != 0) {
		ip = arg.val.n;
		continue;
	  }
	  break;
	case JIF:
	  if (desempilha(pil).val.n == 0) {
		ip = arg.val.n;
		continue;
	  }
	  break;
	case CALL:
	  a.t = NUM;
	  a.val.n = ip;
	  empilha(exec, a);
	  ip = arg.val.n;
	  continue;
	case RET:
	  ip = desempilha(exec).val.n;
	  break;

	case EQ:
	  if (desempilha(pil).val.n == desempilha(pil).val.n)
		empilha(pil, (OPERANDO) {NUM,{1}});
	  else
		empilha(pil, (OPERANDO) {NUM,{0}});
	  break;

	case GT:
	  if (desempilha(pil).val.n < desempilha(pil).val.n)
		empilha(pil, (OPERANDO) {NUM,{1}});
	  else
		empilha(pil, (OPERANDO) {NUM,{0}});
	  break;

	case GE:
	  if (desempilha(pil).val.n <= desempilha(pil).val.n)
		empilha(pil, (OPERANDO) {NUM,{1}});
	  else
		empilha(pil, (OPERANDO) {NUM,{0}});
	  break;

	case LT:
	  if (desempilha(pil).val.n > desempilha(pil).val.n)
		empilha(pil, (OPERANDO) {NUM,{1}});
	  else
		empilha(pil, (OPERANDO) {NUM,{0}});
	  break;

	case LE:
	  if (desempilha(pil).val.n >= desempilha(pil).val.n)
		empilha(pil, (OPERANDO) {NUM,{1}});
	  else
		empilha(pil, (OPERANDO) {NUM,{0}});
	  break;

	case NE:
	  if (desempilha(pil).val.n != desempilha(pil).val.n)
		empilha(pil, (OPERANDO) {NUM,{1}});
	  else
		empilha(pil, (OPERANDO) {NUM,{0}});
	  break;

	case STO:
	  m->Mem[arg.val.n+m->bp[m->ib]] = desempilha(pil);
	  break;

	case RCL:
	  empilha(pil,m->Mem[arg.val.n+m->bp[m->ib]]);
	  break;

	case END:
	  pil->topo = 0;
	  return;

	case PRN:
  // desempilha e imprime o topo
  a = desempilha(pil);
  if (a.t == CELL)
    printf("Terreno: %d\n Vazio: %d\n Cristais: %d\n Base :%d\n", a.Valor.c.terreno, a.Valor.c.vazia, a.Valor.c.nCristais, a.Valor.c.base);
  else if (a.t == BOOL)
  {
    int b = a.Valor.b;
    if (b == 0)
      printf("false\n");
    else
      printf("true\n");
  } else
    printf("%d\n", a.Valor.n);
  empilha(pil, a);
	  break;

	case ENTRY:
	  new_frame(m, arg.val.n);
	  break;

	case LEAVE:
	  del_frame(m);
	  break;
	}

	D(imprime(pil,5));
	D(puts("\n"));

	ip++;
  }
}
