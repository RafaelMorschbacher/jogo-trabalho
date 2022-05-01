#ifndef BIBJOGO_H
#define BIBJOGO_H

#define screenWidth 1000
#define VELOCIDADE_INICIAL 4

#define TRUE 1
#define FALSE 0
#define MAX_INIMIGOS 30
#define VELOCIDADE_INIMIGO 1

//explosao
#define NUM_FRAMES_LINE 7
#define NUM_LINES 1

#include <stdio.h>
#include <raylib.h>
#include <string.h>
#include <math.h>
#include <time.h>


// structs
typedef struct tiro {
    Rectangle posicao;
    Texture2D textura;
    int atirando;
    int velocidade;
    int inclinacao;
    //int numBalas;
    } TIRO;

typedef struct personagem {
    Rectangle posicao;
    Texture2D textura;
    int velocidadeBase;
    int velocidadeAtual;
    int inclinacao;
    int vidas;
    TIRO tiro;
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
    float velocidade;
    int orientacao;             //indo para cima(o), baixo(180), direita(90), esquerda(270)
    int colisao;
    char cor;
    int vivo;
    TIRO tiro;
    } INIMIGO;

typedef struct obstaculo {
Rectangle posicao;
int destruido;
} OBSTACULO;



// funcoes ------------------------------------------------------------------------------------------------------------------

// funcoes menu
float alignCenterFont (char *v, int i, int fontSize, Font arcade);
int readLevel (FILE *level, float *positionX, float *positionY, char *tipo);

// funcoes movimentacao
void checaColisao(PERSONAGEM *personagem, Rectangle obstaculo, Rectangle posicaoInicial);

void checaColisaoArray(INIMIGO *inimigos, PERSONAGEM *personagem, OBSTACULO obstaculos[], int numObstaculos, Rectangle posicaoInicial, int nroInimigos);
void administraPowerUp(POWERUP *powerUp, PERSONAGEM *personagem, OBSTACULO *obstaculos, int numObstaculos, int larguraTela, int alturaTela);
void atualizaPosicao(PERSONAGEM *personagem , Texture personagemRight, Texture personagemLeft, Texture personagemUp, Texture personagemDown);
void desenhaCabecalho(PERSONAGEM *personagem, Texture2D iconeVidas,Font arcade, char *v);
int spawnParede(POWERUP *powerUp, OBSTACULO obstaculos[], int numObstaculos);
void administraTiro(PERSONAGEM *personagem, int larguraTela, int alturaTela, OBSTACULO obstaculos[] ,int nroBlocos, INIMIGO inimigos[], int nroInimigos, int *maxInimigos,int *inimigosMortos, int *inimigosEmTela);

// funcoes inimigos
void criaInimigos(INIMIGO *inimigos, int nroInimigos, Texture inimigoTex, PERSONAGEM *personagem, OBSTACULO obstaculo[], int nroBlocos, char cor);
void modoInimigos (INIMIGO *inimigo, PERSONAGEM *personagem);

void movInimigos (INIMIGO *inimigo, Rectangle posicaoInicial, PERSONAGEM *personagem , int i, int colisaoInimigoCenario, int colisaoDoInimigo, char corInimigo, Texture inimigoRedUp, Texture inimigoRedDown, Texture inimigoRedLeft, Texture inimigoRedRight, Texture inimigoGreenUp, Texture inimigoGreenDown, Texture inimigoGreenLeft, Texture inimigoGreenRight);
int checaColisaoInimigos(int numeroDeInimigos, INIMIGO inimigos[], PERSONAGEM *personagem, int numeroInimigo, OBSTACULO obstaculo[], int nroBlocos);

// funcoes niveis
void criandoMapa(FILE *fileLevel, float *positionX, float *positionY, char *tipo, OBSTACULO obstaculos[], int *nroBlocos, PERSONAGEM *personagem);
void criandoInimigos(clock_t tempo[], int *aux, int *nroInimigos, int *nroBlocos, char *corInimigo, INIMIGO inimigos[], Texture inimigoRedDown, Texture inimigoGreenDown, OBSTACULO obstaculos[], PERSONAGEM *personagem);
void movendoInimigos (int screenHeight, int *nroInimigos, int *nroBlocos, int *colisaoInimigoCenario, int *colisaoDoInimigo, INIMIGO inimigos[], PERSONAGEM *personagem, OBSTACULO obstaculos[], char *corInimigo, Texture inimigoRedUp, Texture inimigoRedDown, Texture inimigoRedLeft, Texture inimigoRedRight, Texture inimigoGreenUp, Texture inimigoGreenDown, Texture inimigoGreenLeft, Texture inimigoGreenRight);
void movendoPersonagem (PERSONAGEM *personagem,int *nroBlocos, int *nroInimigos, int screenHeight, INIMIGO inimigos[], OBSTACULO obstaculos[], Texture personagemRight, Texture personagemLeft, Texture personagemUp, Texture personagemDown);
void administraTiroInimigos(INIMIGO *inimigo, int larguraTela, int alturaTela, OBSTACULO obstaculos[], int nroBlocos, PERSONAGEM *personagem);

#endif
