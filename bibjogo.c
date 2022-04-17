#include <stdio.h>
#include <raylib.h>
#include <string.h>
#include "bibjogo.h"

//VARIÁVEIS ------------------------------------------------------------------------------------------------------------------------
//variaveis menu
char menuOptions[3][10] =  {"Novo Jogo", "Continuar","Sair"};
float u;
char tipo = ' ';
int estadoChave = 1;

//variaveis obstaculos
float positionX = 0;                  //coordenada x de um objeto na tela
float positionY = 1;                  //coordenada y de um objeto na tela
float brickWall[600][2] = {0};
int nroBlocos = 0;

//variaveis movimentação
int positionPlayer[1][2] = {0};

//mov
Rectangle obstaculos[600] ={{0,0,0,0}}; //posX, posY, largura, altura


//TYPESDEFS ------------------------------------------------------------------------------------------------------------------------


//STRUCTS --------------------------------------------------------------------------------------------------------------------------

//Estrutura Personagem

// FUNÇÕES GERAIS ------------------------------------------------------------------------------------------------------------------
float alignCenterFont (char *v, int i, int fontSize, Font arcade) { 
    float width;
    Vector2 tamText = MeasureTextEx(arcade, v, fontSize, 1);
    width = ((screenWidth-tamText.x)/2);
    return width;
}

// FUNÇÕES MENU --------------------------------------------------------------------------------------------------------------------

// FUNÇOES NOVO JOGO ----------------------------------------------------------------------------------------------------------------
// -- carregando mapa
int readLevel (FILE *level, float *positionX, float *positionY, char *tipo) { // função que le arquivo, 1 caractere por vez, e descobre qual é
    float xFuncao = *positionX;
    float yFuncao = *positionY;
    char conteudoTipo = *tipo;
    char leitura;
    
    do {
        fscanf(level,"%c",&leitura);
        switch(leitura) {

          case '\n':leitura = '-'; // para não ser contabilizados os ENTER do arquivo
                    break;
          case '-': xFuncao++;
                    break;
          case 'T': xFuncao++;
                    conteudoTipo = 'T';
                    break;
          case '#': xFuncao++;
                    conteudoTipo = '#';
                    break;
          default:  break;
        }
        if (xFuncao == 41) {
            xFuncao = 1;
            yFuncao++;
        }

    } while (leitura == '-' && (feof(level) == 0) ); // sai da função quando leitura não é mais '-'. enquanto for, continua lendo
                                                     // só vai sair no caso de # ou T. a funçao só para nesses dois casos, e a execução retorna pro main
    *positionX = xFuncao;  //muda a coordenada x do prox objeto
    *positionY = yFuncao;  //muda a coordenada y do prox objeto
    *tipo = conteudoTipo;
    return feof(level);
}

// funcoes MOVIMENTAÇÃO
void checaColisao(PERSONAGEM *personagem, Rectangle *obstaculo, Rectangle posicaoInicial) {
    if(CheckCollisionRecs(personagem->posicao,*obstaculo)) {
        personagem->posicao = posicaoInicial;
        printf("dentro da função coordenadas são %f e %f\n", obstaculo->x, obstaculo->y);
        printf("COLIDIU  \n");
    }
}

void checaColisaoArray(PERSONAGEM *personagem, Rectangle *obstaculos, int numObstaculos, Rectangle posicaoInicial) {
        for(int i=0; i<numObstaculos; i++) {
            checaColisao(&(*personagem), &obstaculos[i], posicaoInicial);
          //  printf("o numero de obstaculos é %i\n", numObstaculos);
          //  printf("estou checando o personagem em %f, %f\n", personagem->posicao.x, personagem->posicao.y); 
           // printf("estou checando c o obstaculo em %f, %f\n\n", obstaculos[i].x, obstaculos[i].y);

        }
}

void administraPowerUp(POWERUP *powerUp, PERSONAGEM *personagem, int larguraTela, int alturaTela) {
    if(CheckCollisionRecs((*personagem).posicao, (*powerUp).posicao)) {
        //Power-up coletado
        personagem->velocidadeAtual = personagem->velocidadeBase * 1.5;
        (*powerUp).cooldown = 5*60;
        (*powerUp).ativo = 1;
        (*powerUp).posicao.x = GetRandomValue((*powerUp).posicao.width, larguraTela-(*powerUp).posicao.width);
        (*powerUp).posicao.y = GetRandomValue((*powerUp).posicao.height,alturaTela-(*powerUp).posicao.height); 
    }
    else {
        if(!powerUp->ativo) {
            DrawRectangleRec((*powerUp).posicao, BLANK);
            DrawTexture(powerUp->textura, (*powerUp).posicao.x, (*powerUp).posicao.y, RAYWHITE);
        }
        else {
            powerUp->cooldown--;
        }

        if(powerUp->cooldown <=0) {
            powerUp->ativo = 0;
            personagem->velocidadeAtual = personagem->velocidadeBase;
        }
    }

    //printf("entrei na ADMINISTRA POWER UP 3 \n");
}

void atualizaPosicao(PERSONAGEM *personagem , Texture personagemRight, Texture personagemLeft, Texture personagemUp, Texture personagemDown) {

    if(IsKeyDown(KEY_RIGHT)) {
        (*personagem).posicao.x += (*personagem).velocidadeAtual;
        (*personagem).textura = personagemRight;
    }

    if(IsKeyDown(KEY_LEFT)) {
        (*personagem).posicao.x -=  (*personagem).velocidadeAtual;
        (*personagem).textura = personagemLeft;
    }

    if(IsKeyDown(KEY_UP)) {
        (*personagem).posicao.y -=  (*personagem).velocidadeAtual;
        (*personagem).textura = personagemUp;
    }

    if(IsKeyDown(KEY_DOWN)) {
        (*personagem).posicao.y +=  (*personagem).velocidadeAtual;
        (*personagem).textura = personagemDown;
    }
    //printf("entrei na ATUALIZA POSIÇÃO 4\n");
}

void desenhaCabecalho(PERSONAGEM *personagem, Texture2D iconeVidas) {
    DrawText("Vidas: ", 100, 15, 20, LIGHTGRAY);
    int espacamento = 0;
    if((*personagem).vidas>0) {
        for(int i= (*personagem).vidas; i>0; i--) {
            DrawTexture(iconeVidas, (180 + espacamento), 10 , RAYWHITE);
            espacamento +=40;
        }
    }
    DrawText("FASE 1", 440, 15, 30, LIGHTGRAY);

    //printf("entrei na DESENHA CABEÇALHO 5 \n");
}