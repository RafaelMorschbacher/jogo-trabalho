#ifndef BIBJOGO_H
#define BIBIJOGO_H

#define screenWidth 1000
#define VELOCIDADE_INICIAL 4

float alignCenterFont (char *v, int i, int fontSize, Font arcade); 
int readLevel (FILE *level, float *positionX, float *positionY, char *tipo);



// structs
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


// funcoes movimentacao 
void checaColisao(Rectangle *personagem, Rectangle *obstaculo, Rectangle posicaoInicial);
void checaColisaoArray(Rectangle *personagem, Rectangle *obstaculos, int numObstaculos, Rectangle posicaoInicial);
void administraPowerUp(POWERUP *powerUp, PERSONAGEM *personagem, int larguraTela, int alturaTela);
void atualizaPosicao(PERSONAGEM *personagem , Texture personagemRight, Texture personagemLeft, Texture personagemUp, Texture personagemDown);
void desenhaCabecalho(PERSONAGEM *personagem, Texture2D iconeVidas); 




#endif