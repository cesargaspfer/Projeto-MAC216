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
void desenhaCelula(int px, int py, int terreno);

// inicializa o apres
void inicializaGraf();

void mostra(int ri);

void DesenhaRobo2 (int qual, int Xantigo, int Yantigo, int Xnovo, int Ynovo);

void NovoRoboDesenho (int time);
