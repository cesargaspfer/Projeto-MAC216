#ifndef INSTR_H
#define INSTR_H
#include "util.h"
#include "arena.h"
/* Códigos das instruções */
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
  ATR,
  ENTRY,
  LEAVE,
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

typedef enum {
  NORTHEAST,
  EAST,
  SOUTHEAST,
  SOUTHWEST,
  WEST,
  NORTHWEST,
  CURRENT
}Dir;

/* Operando */
typedef struct {
  Tipo t;
  union {
	int n;
	int ac;
  double v;
	Celula c;
  Boolean b;
  } Valor;
} OPERANDO;

/* Instrução */
typedef struct {
  OpCode instr;
  OPERANDO op;
} INSTR;
#endif /* INSTR_H*/
