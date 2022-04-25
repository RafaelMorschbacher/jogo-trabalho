#ifndef BIBJOGO_H
#define BIBIJOGO_H

#define screenWidth 1000
#define VELOCIDADE_INICIAL 4

float alignCenterFont (char *v, int i, int fontSize, Font arcade);
int readLevel (FILE *level, float *positionX, float *positionY, char *tipo);



// structs
typedef struct tiro {
    Rectangle posicao;
    Texture2D textura;
    int atirando;
    int velocidade;
    int inclinacao;
    int numBalas;
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


// funcoes movimentacao
void checaColisao(PERSONAGEM *personagem, Rectangle *obstaculo, Rectangle posicaoInicial);
void checaColisaoArray(PERSONAGEM *personagem, Rectangle *obstaculos, int numObstaculos, Rectangle posicaoInicial);
int spawnParede(POWERUP *powerUp, Rectangle obstaculos[], int numObstaculos);
void administraPowerUp(POWERUP *powerUp, PERSONAGEM *personagem, Rectangle *obstaculos, int numObstaculos, int larguraTela, int alturaTela);
void atualizaPosicao(PERSONAGEM *personagem , Texture personagemRight, Texture personagemLeft, Texture personagemUp, Texture personagemDown);
void desenhaCabecalho(PERSONAGEM *personagem, Texture2D iconeVidas);
void administraTiro(PERSONAGEM *personagem, int larguraTela, int alturaTela);



#endif
