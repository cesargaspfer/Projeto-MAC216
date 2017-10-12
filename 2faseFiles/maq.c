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
  "FRE",
  "MOV", // Adicionado codes daqui para baixo
  "ATK",
  "INF",
  "CLT",
  "DEP"
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

Maquina* cria_maquina(INSTR *p) {
  Maquina *m = (Maquina*)malloc(sizeof(Maquina));
  if (!m) Fatal("Memória insuficiente",4);
  m->ip = 0;
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
  // variáveis de iteração
  int i;
  int indexALC = 0;
  // inteiro auxiliar, para iterações e atribuições
  int temp = 0;
  // Operandos auxiliares, para efetuar operações com mais de um operando
  OPERANDO a,b;
  for (i = 0; i < n; i++) {
    // recuperar OpCode da instrução
     OpCode opc = prg[ip].instr;
     //construir OPERANDO (arg) a partir da instrução
	    OPERANDO arg = prg[ip].o;

	D(printf("%3d: %-4.4s %d\n     ", ip, CODES[opc], arg));
  /*
  MOV, ATK, INF estão prontos. Olhar funcionamento.
  */
	switch (opc) {
	  OPERANDO tmp;
	case PUSH:
    //Empilhar um objeto
	  empilha(pil, arg);
	  break;
	case POP:
    // Desempilhar um objeto
	  desempilha(pil);
	  break;
	case DUP:
    // Duplicar o objeto no topo da pilha
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
      empilha(pil, (OPERANDO){NUM,a.Valor.n + b.Valor.n});
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
      empilha(pil, (OPERANDO){NUM, a.Valor.n - b.Valor.n});
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
    // Colocar o ip na instrução dada no argumento
	  ip = arg.Valor.n;
	  continue;
	case JIT:
    // Se o valor no topo da pilha for verdadeiro,
    // posicionar o ip na posição dada pelo argumento
	  if (desempilha(pil).Valor.b != false) {
	    ip = arg.Valor.n;
	    continue;
	  }
	  break;
	case JIF:
  // Se o valor no topo da pilha for false,
  // atribuir ao ip na posição dada pelo argumento
	  if (desempilha(pil).Valor.n == false) {
  		ip = arg.Valor.n;
  		continue;
	  }
	  break;
	case CALL:
    // empilhar o ip atual e atribuir o valor do argumento da função ao ip
	  empilha(exec, (OPERANDO){NUM, ip});
	  ip = arg.Valor.n;
	  continue;
	case RET:
    // retornar o ip para a posição anterior, que foi empilhada por CALL
	  ip = desempilha(exec).Valor.n;
	  break;
	case EQ:
    // Verificar se os dois valores no topo são iguais
	  if (desempilha(pil).Valor.n == desempilha(pil).Valor.n)
      // se forem, então empilha verdadeiro
      empilha(pil, (OPERANDO) {BOOL, true});
	  else
      // se não forem, empilha falso
      empilha(pil, (OPERANDO){BOOL, false});
	  break;
	case GT:
    // verificar se o topo é maior do que a posição abaixo dele
	  if (desempilha(pil).Valor.n < desempilha(pil).Valor.n)
      empilha(pil, (OPERANDO){BOOL, true});
	  else
		  empilha(pil, (OPERANDO){BOOL, false});
	  break;
	case GE:
	  if (desempilha(pil).Valor.n <= desempilha(pil).Valor.n)
		  empilha(pil, (OPERANDO){BOOL, true});
	  else
		  empilha(pil, (OPERANDO){BOOL, false});
	  break;
	case LT:
	  if (desempilha(pil).Valor.n > desempilha(pil).Valor.n)
		  empilha(pil, (OPERANDO){BOOL, true});
	  else
		  empilha(pil, (OPERANDO){BOOL, false});
	  break;
	case LE:
	  if (desempilha(pil).Valor.n >= desempilha(pil).Valor.n)
		  empilha(pil, (OPERANDO){BOOL, true});
	  else
		  empilha(pil, (OPERANDO){BOOL, false});
	  break;
	case NE:
	  if (desempilha(pil).Valor.n != desempilha(pil).Valor.n)
		  empilha(pil, (OPERANDO){BOOL, true});
	  else
		  empilha(pil, (OPERANDO){BOOL, false});
	  break;
	case STO:
	  m->Mem[arg.Valor.n] = desempilha(pil);
	  break;
  case RCL:
	  empilha(pil,m->Mem[arg.Valor.n]);
	  break;
	case END:
    // terminar o programa
	  return;
	case PRN:
    // desempilha e imprime o topo
	  printf("%d\n", desempilha(pil).Valor);
	  break;
  case STL:
    tmp = desempilha(pil);
    empilha(pil, tmp);
    indexALC = 0;
    while (indexALC != arg.Valor.n + 1){
      empilha(pil, desempilha(exec));
      indexALC++;
    }
    empilha(exec, tmp);
    tmp = desempilha(pil);
    indexALC = 0;
    while(indexALC != arg.Valor.n){
      empilha(exec, desempilha(pil));
      indexALC++;
    }
    break;
  case RCE: //Copia na pilha de dados o valor dado por args da pilha de exec
    indexALC = 0;
    while(indexALC != arg.Valor.n){
      empilha(pil, desempilha(exec));
      indexALC++;
    }
    tmp = desempilha(exec);
    empilha(exec, tmp);
    indexALC = 0;
    while (indexALC != arg.Valor.n){
      empilha(exec, desempilha(pil));
      indexALC++;
    }
    empilha(pil, tmp);
    break;
  case ALC:
    indexALC = 0;
    while (indexALC != arg.Valor.n)
    {
      empilha(exec,(OPERANDO) {NUM, {0}});
        indexALC++;
    }
    empilha(exec, arg);
    break;
  case FRE:
    indexALC = desempilha(exec).Valor.n;
    while (indexALC != 0)
    {
      tmp = desempilha(exec);
      indexALC--;
    }
    break;
    /* Implementação das novas instruções, sendo elas:
    MOV, ATK, ... (A CONCLUIR) */

  case MOV:
    Sistema(0, arg.Valor.ac);
    break;
  case ATK:
    Sistema(1, arg.Valor.ac);
    break;
  case INF:
    Sistema(2, 0);
    break;
  case CLT:
    Sistema(3, 0);
    break;
  case DEP:
    Sistema(4, 0);
    break;
	}
	D(imprime(pil,5));
	D(puts("\n"));

	ip++;
  }
}
