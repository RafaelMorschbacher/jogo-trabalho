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
int spawnParede(POWERUP *powerUp, Rectangle obstaculos[], int numObstaculos)
{
    int colisao = 0;
    for(int i=0; i<numObstaculos; i++)
    {
        if(CheckCollisionRecs(obstaculos[i], powerUp->posicao))
        {
            colisao = 1;
        }

    }

    return colisao;
}
void administraPowerUp(POWERUP *powerUp, PERSONAGEM *personagem, Rectangle *obstaculos, int numObstaculos, int larguraTela, int alturaTela)
{
    if(CheckCollisionRecs(personagem->posicao, powerUp->posicao))
    {
        //Power-up coletado
        personagem->velocidadeAtual = personagem->velocidadeBase * 1.5;
        powerUp->cooldown = 5*60;
        powerUp->ativo = 1;
        do{
            powerUp->posicao.x = GetRandomValue(powerUp->posicao.width, GetScreenWidth()-powerUp->posicao.width);
            powerUp->posicao.y = GetRandomValue(powerUp->posicao.height+40,GetScreenHeight()- powerUp->posicao.height);
        }while(spawnParede(&(*powerUp), &(*obstaculos), numObstaculos));


        }
        if(powerUp->ativo)
        {
            powerUp->cooldown--;
        }
        else
        {
                DrawRectangleRec(powerUp->posicao, BLANK);
                DrawTexture(powerUp->textura, powerUp->posicao.x, powerUp->posicao.y, RAYWHITE);
        }

        if(powerUp->cooldown <=0)
        {
            powerUp->ativo = 0;
            personagem->velocidadeAtual = personagem->velocidadeBase;
        }



}

void atualizaPosicao(PERSONAGEM *personagem , Texture personagemRight, Texture personagemLeft, Texture personagemUp, Texture personagemDown) {

    if(IsKeyDown(KEY_RIGHT)) {
        personagem->posicao.x += (*personagem).velocidadeAtual;
        personagem->textura = personagemRight;
        personagem->inclinacao = 0;
    }

    if(IsKeyDown(KEY_LEFT)) {
        personagem->posicao.x -=  (*personagem).velocidadeAtual;
        personagem->textura = personagemLeft;
        personagem->inclinacao = 180;
    }

    if(IsKeyDown(KEY_UP)) {
        personagem->posicao.y -=  (*personagem).velocidadeAtual;
        personagem->textura = personagemUp;
        personagem->inclinacao = 90;
    }

    if(IsKeyDown(KEY_DOWN)) {
        personagem->posicao.y +=  (*personagem).velocidadeAtual;
        personagem->textura = personagemDown;
        personagem->inclinacao = 270;
    }
}

void administraTiro(PERSONAGEM *personagem, int larguraTela, int alturaTela){
    //Atirando ao apertar SPACE
    if(IsKeyReleased(KEY_SPACE) && !personagem->tiro.atirando && personagem->tiro.numBalas>0){
        personagem->tiro.atirando = 1;
        personagem->tiro.posicao.x = personagem->posicao.x +10;
        personagem->tiro.posicao.y = personagem->posicao.y +10;
        personagem->tiro.inclinacao = personagem->inclinacao;
        personagem->tiro.numBalas -= 1;
        printf("%d",  personagem->tiro.numBalas);
    }

    //Animação de tiro
    if(personagem->tiro.atirando){
        DrawRectangleRec(personagem->tiro.posicao, GREEN);
        switch(personagem->tiro.inclinacao){
            case 0:
                personagem->tiro.posicao.x += personagem->tiro.velocidade;
            break;
            case 90:
                personagem->tiro.posicao.y -= personagem->tiro.velocidade;
            break;
            case 180:
                personagem->tiro.posicao.x -= personagem->tiro.velocidade;
            break;
            case 270:
                personagem->tiro.posicao.y += personagem->tiro.velocidade;
            break;

        }

    }

    //Destruição do tiro
    if(personagem->tiro.posicao.x > larguraTela || personagem->tiro.posicao.x < 0 || personagem->tiro.posicao.y >= alturaTela || personagem->tiro.posicao.y < 0){
        personagem->tiro.atirando = 0;
    }
}

void desenhaCabecalho(PERSONAGEM *personagem, Texture2D iconeVidas) {
    DrawText("Vidas: ", 100, 15, 20, LIGHTGRAY);
    int espacamento = 0;
    if((*personagem).vidas>0) {
        for(int i= (*personagem).vidas; i>0; i--) {
            DrawTexture(iconeVidas, (180 + espacamento), 2 , RAYWHITE);
            espacamento +=40;
        }
    }
    DrawText("FASE 1", 440, 5, 30, LIGHTGRAY);

    //printf("entrei na DESENHA CABEÇALHO 5 \n");
}
