// Struct com atributos do robô da visualização
typedef struct {
  int pi, pj;
  int i, j;
  int vi, vj;
  int exercito;
} Robot;
// Struct com atributos de uma célula da visualização
typedef struct {
  int tipoTerreno;
  int px;
  int py;
} Cell;
// função anda, que recebe o indice de um "Robot" do Vetor
// em controle.c
void anda(int ri);

// função atualiza, que recebe o indice de um "Robot" do Vetor
// em controle.c e muda os atributos do objeto Robot
void atualiza(int ri);

// essa função escreve as instruções para desenhar a célula segundo o protocolo
// do apres
void desenhaCelula(int px, int py, int terreno);

// inicializa o apres
void inicializaGraf();

void mostra(int ri);
