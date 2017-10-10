#include "arena.h"
#include "util.h"
/* Códigos das instruções */
/* Adição de MOV e ATR (por enquanto) */
typedef enum {
  PUSH,
  POP,
  DUP,
  ADD,
  SUB,
  MUL,
  DIV,
  JMP,
  JIT,
  JIF,
  CALL,
  RET,
  STS,
  RCS,
  EQ,
  GT,
  GE,
  LT,
  LE,
  NE,
  STO,
  RCL,
  END,
  PRN,
  STL,
  RCE,
  ALC,
  FRE,
  // ATR,
  MOV,
  ATK,
  DEP,
  INF,
  CLT
} OpCode;

/* Tipos dos operandos */
typedef enum {
  NUM,
  ACAO,
  VAR,
  CELL,
  BOOL,
  NONE
} Tipo;
/* Operando, agora com o atributo Tipo */

/***************************************
* t = tipo da variavel
* n = valor da variavel de tipo NUM
* ac = valor da variavel do tipo ACAO
* v = variavel do tipo VAR
* c = célula do tipo CELL
* b = valor booleano BOOL
***************************************/
typedef struct {
   Tipo t;
  union Valor {
 	  int n;        
 	  int ac;
 	  int v;
    Celula c;
    Boolean b;
  } Valor;
 } OPERANDO;
/*typedef int OPERANDO;*/

/* Instrução */
typedef struct {
  OpCode instr;
  Tipo t;
  int* values;
} INSTR;
