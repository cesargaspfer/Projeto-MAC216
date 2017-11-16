// Struct com atributos do robô da visualização
typedef struct {
  // coordenadas da célula de origem do robo
  int oi, oj;
  // coordenadas da célula de destino do robô
  int di, dj;
  // vetor de velocidade do robô
  int vi, vj;
  //exército ao qual o robô pertence
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
void desenhaCelula(int x, int y, int terreno);
// desenhar uma base na célula x,y
void desenhaBase(int x, int y, int exercito);
// desenhar um cristal na célula x,y
void desenhaCristal(int x, int y, int n);
// desenhar uma arma na célula x,y
void desenhaArma(int x, int y);
// remover o desenho de um item na célula x,y
void removeItem(int x, int y);

void desenhaRobo (int exercito, int index, int i, int j);

// inicializa o apres
void inicializaGraf();

void mostra(int ri);
