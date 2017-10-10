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

Maquina *cria_maquina(INSTR *p, int sizes[]) {
  Maquina *m = (Maquina*)malloc(sizeof(Maquina));
  if (!m) Fatal("Memória insuficiente",4);
  m->ip = 0;
  int i = 0;
  // para alocar dinamicamente, precisamos do tamanho do values de cada instr
  // falta alocar
  m->prog = p;
  return m;
}

void destroi_maquina(Maquina *m) {
  free(m);
}

#define ip (m->ip)
#define pil (&m->pil)
#define exec (&m->exec)
#define prg (m->prog)

void exec_maquina(Maquina *m, int n) {
  int i;
  int indexALC = 0;
  int temp = 0;
  for (i = 0; i < n; i++) {
	   OpCode   opc = prg[ip].instr;
     //construir OPERANDO (arg)
	    Tipo t = prg[ip].t;
      printf("%d \n", sizeof(prg[ip].values));
      OPERANDO arg;
      arg.t = t;
      /* Qual tipo está chegando aqui? (Tipos são enum, e enums são números com nomes. 
      Então no nosso código, NUM = 0 (...) )*/
      switch (t)
      {
        case 0:
          printf("NUM \n");
          break;
        case 1:
          printf("ACAO \n");
          break;
        case 2:
          printf("VAR \n");
          break;
        case 3:
          printf("CELL \n");
          break;
        case 4:
          printf("BOOL \n");
          break;
        case 5:
          printf("NONE \n");
          break;
        default:
          printf("Outro tipo não definido no switch \n");
          break;
      }
      //arg.Valor.n = arrv[0];

	D(printf("%3d: %-4.4s %d\n     ", ip, CODES[opc], arg));

	switch (opc) {
	  OPERANDO tmp;
	case PUSH:
	  empilha(pil, arg);
	  break;
	case POP:
	  //desempilha(pil);
	  break;
	case DUP:
	  /*tmp = desempilha(pil);
	  empilha(pil, tmp);
	  empilha(pil, tmp);*/
	  //break;
	/*case ADD:
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
	*/case END:
	  return;
	case PRN:
	  printf("%d\n", desempilha(pil));
	  break;
  /*case STL:
    temp = desempilha(pil); //
    empilha(pil, temp);
    indexALC = 0;
    while (indexALC != arg + 1){
      empilha(pil, desempilha(exec));
      indexALC++;
    }
    empilha(exec, temp);
    temp = desempilha(pil);
    indexALC = 0;
    while(indexALC != arg){
      empilha(exec, desempilha(pil));
      indexALC++;
    }
    break;
  case RCE: //Copia na pilha de dados o valor dado por args da pilha de exec
    indexALC = 0;
    while(indexALC != arg){
      empilha(pil, desempilha(exec));
      indexALC++;
    }
    temp = desempilha(exec); //
    empilha(exec, temp);
    indexALC = 0;
    while (indexALC != arg){
      empilha(exec, desempilha(pil));
      indexALC++;
    }
    empilha(pil, temp);
    break;
  case ALC:
    indexALC = 0;
    while (indexALC != arg)
    {
      empilha(exec, 0);
        indexALC++;
    }
    empilha(exec, arg);
    break;
  case FRE:
    indexALC = desempilha(exec);
    while (indexALC != 0)
    {
      temp = desempilha(exec);
      indexALC--;
    }
    break;*/

    /* Implementação das novas instruções, sendo elas:
    MOV, ATK, ... (A CONCLUIR)
    */
  case MOV:
    Sistema(0, arg.Valor.ac);
    break;
  case ATK:
    Sistema(1, arg.Valor.ac);
    break;
  case INFO:
    Sistema(2, 0);
    break;
  case CLT:
    Sistema(3, );
    break;
  case DEP:
    Sistema(4, );
    break;
	}
	D(imprime(pil,5));
	D(puts("\n"));

	ip++;
  }
}
