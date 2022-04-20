#ifndef BIBJOGO_H
#define BIBIJOGO_H


#define screenWidth 1000
#define VELOCIDADE_INICIAL 4
#define TRUE 1
#define FALSE 0 




// structs ----------------------------------------------------------------------------------------------------------------
typedef struct personagem {
    Rectangle posicao;
    Texture2D textura;
    int velocidadeBase;
    int velocidadeAtual;
    int vidas;
    } PERSONAGEM;

typedef struct powerUp {
    Rectangle posicao;
    int ativo;
    int cooldown;
    Texture2D textura;
    } POWERUP;

typedef struct inimigo {
    char modo;                  // Patrulha(N) ou Perseguição (P)
    char sentidoDesloc;         // Aleatorio ou seguindo
    Rectangle posicao;          //x,y, width, height 
    Texture2D textura;
    int velocidade;
    int orientacao;         //indo para cima(o), baixo(180), direita(90), esquerda(270)
    char cor; 
     
    } INIMIGO;

// funcoes ------------------------------------------------------------------------------------------------------------------

// funcoes menu
float alignCenterFont (char *v, int i, int fontSize, Font arcade); 
int readLevel (FILE *level, float *positionX, float *positionY, char *tipo);

// funcoes movimentacao 
void checaColisao(PERSONAGEM *personagem, Rectangle *obstaculo, Rectangle posicaoInicial);
void checaColisaoArray(PERSONAGEM *personagem, Rectangle *obstaculos, int numObstaculos, Rectangle posicaoInicial,int nroInimigos);
void administraPowerUp(POWERUP *powerUp, PERSONAGEM *personagem, int larguraTela, int alturaTela);
void atualizaPosicao(PERSONAGEM *personagem , Texture personagemRight, Texture personagemLeft, Texture personagemUp, Texture personagemDown);
void desenhaCabecalho(PERSONAGEM *personagem, Texture2D iconeVidas); 

// funcoes inimigos
void criaInimigos(int nroInimigos, Texture inimigoTex, PERSONAGEM *personagem, Rectangle *obstaculo, int nroBlocos, char cor);
void modoInimigos (PERSONAGEM *personagem , int nroInimigos);
void movInimigos (Rectangle posicaoInicial, PERSONAGEM *personagem , int i, int colisaoInimigoCenario, int colisaoDoInimigo, char corInimigo, Texture inimigoRedUp, Texture inimigoRedDown, Texture inimigoRedLeft, Texture inimigoRedRight, Texture inimigoGreenUp, Texture inimigoGreenDown, Texture inimigoGreenLeft, Texture inimigoGreenRight);
int checaColisaoInimigos(PERSONAGEM *personagem, int numeroInimigo, Rectangle *obstaculo, int nroBlocos);
#endif