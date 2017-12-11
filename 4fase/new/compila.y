/* Compilador */

%{
#include <stdio.h>
#include <math.h>
#include "symrec.h"
#include "acertos.h"
#include "instr.h"

int yylex();
void yyerror(char const *);
int compila(FILE *, INSTR *);
static int ip  = 0;					/* ponteiro de instruções */
static int mem = 6;					/* ponteiro da memória */
static INSTR *prog;
static int parmcnt = 0;		/* contador de parâmetros */
static int ipcheck = 0;
static int ipcheca = 0;

void AddInstr(OpCode op, int val) {
  prog[ip++] = (INSTR) {op,  {NUM, {val}}};
}
%}

/* O que é interpretado pode ser tanto número como letra/simbolo */
%union {
  double val;
  /* symrec *cod; */
  char cod[30];
}

/* %type  Expr */
/* Simbolos terminais/ definições (#define) */
%token <val>  NUMt
%token <cod> ID
%token ADDt SUBt MULt  ASGN OPEN CLOSE RETt EOL
%token EQt NEt LTt LEt GTt GEt ABRE FECHA SEP
%token IF IFELSE WHILE FUNC PRINT
%token INFORMACAO PONTO
%token MOVA ATAQUE COLETE DEPOSITE
%token <val> DIRECAO
%token <val> ATRIBUTO
%right ASGN
%left ADDt SUBt
%left MULt DIVt
%left NEG
%right PWR
%left LTt GTt LEt GEt EQt NEt


/* Gramatica */
%%

Programa: Comando
       | Programa Comando
	   ;

Comando: Expr EOL
       | Cond
       | Loop
       | Func
	   | PRINT Expr EOL { AddInstr(PRN, 0);}
	   | RETt EOL {
		 	     AddInstr(LEAVE, 0);
			     AddInstr(RET, 0);
 			  }
	   | RETt OPEN  Expr CLOSE EOL {
		 	     AddInstr(LEAVE, 0);
			     AddInstr(RET,0);
 		      }
	   /* | EOL {printf("--> %d\n", ip);} */
;

Expr: NUMt {  AddInstr(PUSH, $1);}
    | ID   {
	         symrec *s = getsym($1);
			 if (s==0) s = putsym($1); /* não definida */
			 AddInstr(RCL, s->val);
	       }
	| ID ASGN Expr {
	         symrec *s = getsym($1);
			 if (s==0) s = putsym($1); /* não definida */
			 AddInstr(STO, s->val);
 		 }
   | MOVA OPEN DIRECAO CLOSE   {
          AddInstr(MOV, $3);
       }
  | ATAQUE OPEN DIRECAO CLOSE   {
          AddInstr(ATK, $3);
       }
  | COLETE OPEN DIRECAO CLOSE   {
          AddInstr(CLT, $3);
       }
  | DEPOSITE OPEN DIRECAO CLOSE   {
            AddInstr(DEP, $3);
       }

   | INFORMACAO OPEN DIRECAO CLOSE {
       AddInstr(INF, $3);
  		 }
	 | ID PONTO ATRIBUTO  {
	          symrec *s = getsym($1);
	 		 if (s==0) s = putsym($1);
	 		 AddInstr(PUSH, s->val);
	 		 AddInstr(ATR, $3);
 	 	 }
	| Chamada
    | Expr ADDt Expr { AddInstr(ADD,  0);}
	| Expr SUBt Expr { AddInstr(SUB,  0);}
	| Expr MULt Expr { AddInstr(MUL,  0);}
	| Expr DIVt Expr { AddInstr(DIV,  0);}
    | '-' Expr %prec NEG  { printf("  {CHS,  0},\n"); }
	| OPEN Expr CLOSE
	| Expr LTt Expr  { AddInstr(LT,   0);}
	| Expr GTt Expr  { AddInstr(GT,   0);}
	| Expr LEt Expr  { AddInstr(LE,   0);}
	| Expr GEt Expr  { AddInstr(GE,   0);}
	| Expr EQt Expr  { AddInstr(EQ,   0);}
	| Expr NEt Expr  { AddInstr(NE,   0);}
;
Cond: IF OPEN  Expr {

			   
  	  	 	   salva_end(ip);
  	  	 	   // salva_end(ip);
			   AddInstr(JIF, 0);
			   // ipcheck = ip;


 		 }
		 CLOSE  Bloco {

		   // salva_end(ip);
		   ipcheca = pega_end(); // onde tá o jif
		   salva_end(ip); // onde tá o jmp
		   AddInstr(JMP, ip);  // jmp aponta pro bloco do if
		   prog[ipcheca].op.Valor.n = ip;
		   // salva_end(ip);
		   // salva_end(ip);
		   // ip = ip + prog[pega_atu()].op.val.n;

		 };


Cond: Cond IFELSE Bloco {

	       prog[pega_end()].op.Valor.n = ip;
	       // ipcheca = ip;
	       // if (prog[ipcheck].op.Valor.n != 0) AddInstr(JMP, ip);


		 };


Loop: WHILE OPEN  {salva_end(ip);}
	  		Expr  { salva_end(ip); AddInstr(JIF,0); }
	  		CLOSE Bloco {
			  int ip2 = pega_end();
			  AddInstr(JMP, pega_end());
			  prog[ip2].op.Valor.n = ip;
			};

Bloco: ABRE Comandos FECHA ;

Comandos: Comando
    | Comandos Comando
	;
Func: FUNC ID
	  {
		salva_end(ip);
		AddInstr(JMP,  0);
		symrec *s = getsym($2);
		if (s==0) s = putsym($2);
		else {
		  yyerror("Função definida duas vezes\n");
		  YYABORT;
		}
		s->val = ip;
	  } OPEN
	  {
		newtab(0);
	  }
	  Args CLOSE  Bloco
	  {
		AddInstr(LEAVE, 0);
		AddInstr(RET, 0);
		prog[pega_end()].op.Valor.n = ip;
		deltab();
	  }
	  ;
Args:
	| ID {
	  	 putsym($1);
	  }
    | Args SEP ID {
	  	 putsym($3);
	  }
	;

Chamada: ID OPEN
		 {
			 parmcnt = 0;
			 /* posição da memória mais avançada */
		 }
		 ListParms
		 {
		   symrec *s = getsym($1);
		   if (s == 0) {
			 yyerror("Função não definida\n");
			 YYABORT;
		   }
		   AddInstr(ENTRY, lastval());
		   /* Cópia dos parâmetros */
		   while (parmcnt > 0)
			 AddInstr( STO, --parmcnt);
		   AddInstr(CALL, s->val);
		 }
	  	 CLOSE ;


ListParms:
	| Expr { parmcnt++;}
	| Expr { parmcnt++;} SEP ListParms
;

%%
extern FILE *yyin;

void yyerror(char const *msg) {
  fprintf(stderr,"ERRO: %s",msg);
}

int compilador(FILE *cod, INSTR *dest) {
  int r;
  yyin = cod;
  prog = dest;
  r = yyparse();
  AddInstr(END,0);
  return r;
}

/* int main(int ac, char **av) */
/* { */
/*   ac --; av++; */
/*   if (ac>0) */
/* 	yyin = fopen(*av,"r"); */

/*   yyparse(); */
/*   return 0; */
/* } */
