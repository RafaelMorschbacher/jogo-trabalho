#include <stdio.h>
#include <raylib.h>
#include <string.h>
#include <time.h>
#include <math.h>
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
Rectangle obstaculos[600] ={{0,0,0,0}}; //posX, posY, largura, altura
//variaveis movimentação
int positionPlayer[1][2] = {0};
//variaveis inimigos
INIMIGO inimigos[100] = {}; //como escolher o tamanho?
int nroInimigos = 0; 
clock_t tempo[2]; 
int aux = 1;
char corInimigo; 
int colisaoDoInimigo = FALSE; 
int colisaoInimigoCenario = FALSE; 


// FUNÇÕES GERAIS  ------------------------------------------------------------------------------------------------------------------
float alignCenterFont (char *v, int i, int fontSize, Font arcade) { 
    float width;
    Vector2 tamText = MeasureTextEx(arcade, v, fontSize, 1);
    width = ((screenWidth-tamText.x)/2);
    return width;
}

// FUNÇOES NOVO JOGO 
//cabeçalho
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
}

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

// FUNCOES MOVIMENTAÇÃO & power UP
void checaColisao(PERSONAGEM *personagem, Rectangle *obstaculo, Rectangle posicaoInicial) {
    if(CheckCollisionRecs(personagem->posicao,*obstaculo)) {
        personagem->posicao = posicaoInicial;
    }
}

void checaColisaoArray(PERSONAGEM *personagem, Rectangle *obstaculos, int numObstaculos, Rectangle posicaoInicial, int nroInimigos) {
        for(int i=0; i<numObstaculos; i++) {
            checaColisao(&(*personagem), &obstaculos[i], posicaoInicial);
        }
        for (int i=0;i<nroInimigos;i++) {
            checaColisao(&(*personagem), &(inimigos[i].posicao), posicaoInicial);
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
}

//FUNÇÕES ESPECIFICAS INIMIGOS

void criaInimigos(int nroInimigos, Texture inimigoTex, PERSONAGEM *personagem, Rectangle *obstaculo, int nroBlocos, char cor) {
    *(&(inimigos[nroInimigos-1].posicao.width)) = 25;
    *(&(inimigos[nroInimigos-1].posicao.height)) = 25;
    *(&(inimigos[nroInimigos-1].textura)) = inimigoTex;
    *(&(inimigos[nroInimigos-1].velocidade)) = VELOCIDADE_INICIAL;
    *(&(inimigos[nroInimigos-1].orientacao)) = (nroInimigos % 4) * 90; 

    do {
        *(&(inimigos[nroInimigos-1].posicao.x)) = (float)GetRandomValue(25, screenWidth-25);
        *(&(inimigos[nroInimigos-1].posicao.y)) = (float)GetRandomValue(75, 650-25);
    } while ((checaColisaoInimigos(personagem, (nroInimigos-1), obstaculo, nroBlocos)) == TRUE); //para q nao seja criado em cima de onde n da
    
    if (cor == 'G')
        *(&(inimigos[nroInimigos-1].cor)) = 'G';
    else if (cor == 'R')
        *(&(inimigos[nroInimigos-1].cor)) = 'R';
}
// FUNÇÃO P MODO (ALEATORIO OU PERSEGUIÇÃO)
void modoInimigos (PERSONAGEM *personagem , int i) {
    if ((fabs(*(&(inimigos[i].posicao.x)) - (*personagem).posicao.x) < 7 )|| fabs(*(&(inimigos[i].posicao.y)) - (*personagem).posicao.y) < 7){
        if ((colisaoInimigoCenario == FALSE) && (colisaoDoInimigo == FALSE))
            *(&(inimigos[i].modo)) = 'P'; 
    }
    else {
        *(&(inimigos[i].modo)) = 'N';;
    }
}

void movInimigos (Rectangle posicaoInicial, PERSONAGEM *personagem , int i, int colisaoInimigoCenario, int colisaoDoInimigo, char corInimigo, Texture inimigoRedUp, Texture inimigoRedDown, Texture inimigoRedLeft, Texture inimigoRedRight, Texture inimigoGreenUp, Texture inimigoGreenDown, Texture inimigoGreenLeft, Texture inimigoGreenRight) {
    
// OLHAR = TEM ALGUM OBSTACULO NA MINHA ORIENTAÇÃO? NÃO? VIRAR PRA ESSE LADO
// (IF P) ENQUANTO TA NA MESMA LINHA, ANDA NESSA DIREÇÃO
//------ isso n atrapalha o andamento do algoritimo?
// QUANDO n estiver mais, dependendo da posição, troca a orientação



if ( *(&(inimigos[i].modo)) == 'P') {
    if ((colisaoInimigoCenario == TRUE) || (colisaoDoInimigo == TRUE)) {
            *(&(inimigos[i].modo)) = 'N';
        }
    else {
        float diferencaPosicoesX = *(&(inimigos[i].posicao.x)) - (*personagem).posicao.x;
        float diferencaPosicoesY = *(&(inimigos[i].posicao.y)) - (*personagem).posicao.y;

        if ((fabs(*(&(inimigos[i].posicao.x)) - (*personagem).posicao.x) < 7 )) {
            if (diferencaPosicoesY >= 0)
                *(&(inimigos[i].orientacao)) = 0;
            else if (diferencaPosicoesY < 0)
                *(&(inimigos[i].orientacao)) = 180;
        }
        else if ((fabs(*(&(inimigos[i].posicao.y)) - (*personagem).posicao.y) < 7 )) {
            if (diferencaPosicoesX >= 0)
                *(&(inimigos[i].orientacao)) = 270;
            else if (diferencaPosicoesX < 0)
                *(&(inimigos[i].orientacao)) = 90;   
        }
    }
}

    if ( *(&(inimigos[i].modo)) == 'N') { //Normal (Patrulha)
        if (colisaoInimigoCenario == TRUE || colisaoDoInimigo == TRUE) {
            switch (*(&(inimigos[i].orientacao))) {
                case 0: *(&(inimigos[i].posicao.y))+= 5; break;
                case 90: *(&(inimigos[i].posicao.x))-= 5; break;
                case 180: *(&(inimigos[i].posicao.y))-= 5; break;
                case 270:  *(&(inimigos[i].posicao.x))+= 5; break; 
            }
            *(&(inimigos[i].orientacao)) -= 90*(pow(-1,i)); 
        }

    } 
        if (*(&(inimigos[i].orientacao)) == -90)
            *(&(inimigos[i].orientacao)) =270; 
        if (*(&(inimigos[i].orientacao)) == 360)
            *(&(inimigos[i].orientacao)) = 0; 

        switch(*(&(inimigos[i].orientacao))) {
            case 0: {
                *(&(inimigos[i].posicao.y))-= 0.4; 
                if (*(&(inimigos[i].cor)) == 'R') 
                    *(&(inimigos[i].textura)) =  inimigoRedUp; 
                if (*(&(inimigos[i].cor)) == 'G') 
                    *(&(inimigos[i].textura)) =  inimigoGreenUp;
                break; 
            } 
            case 90: {
                *(&(inimigos[i].posicao.x))+= 0.4; 
                if (*(&(inimigos[i].cor)) == 'R') 
                    *(&(inimigos[i].textura)) =  inimigoRedRight; 
                if (*(&(inimigos[i].cor)) == 'G') 
                    *(&(inimigos[i].textura)) =  inimigoGreenRight;
                break; 
            } 
            case 180: {
                *(&(inimigos[i].posicao.y))+= 0.4; 
                if (*(&(inimigos[i].cor)) == 'R') 
                    *(&(inimigos[i].textura)) =  inimigoRedDown; 
                if (*(&(inimigos[i].cor)) == 'G') 
                    *(&(inimigos[i].textura)) =  inimigoGreenDown;
                break;
            }                 
            case 270: {
                *(&(inimigos[i].posicao.x))-= 0.4; 
                if (*(&(inimigos[i].cor)) == 'R') 
                    *(&(inimigos[i].textura)) =  inimigoRedLeft; 
                if (*(&(inimigos[i].cor)) == 'G') 
                    *(&(inimigos[i].textura)) =  inimigoGreenLeft; 
                break;
            }
        
        }
}

int checaColisaoInimigos(PERSONAGEM *personagem, int numeroInimigo, Rectangle *obstaculo, int nroBlocos) {
    int colisaoDoInimigo = FALSE; 

    for (int i = 0; i<nroBlocos; i++) {
        if ( CheckCollisionRecs(*(&(inimigos[numeroInimigo].posicao)),*(&obstaculo[i]))){               //checa colisao com tijolos
            colisaoDoInimigo = TRUE;
        }
        if ( CheckCollisionRecs(*(&(inimigos[numeroInimigo].posicao)),(*personagem).posicao)) {          //checa colisao com personagem
            colisaoDoInimigo = TRUE;
        }
        if ( CheckCollisionRecs(*(&(inimigos[numeroInimigo].posicao)),*(&(inimigos[i].posicao))) ) {    //checa colisão com outros tanques
            if (numeroInimigo != i) {
                colisaoDoInimigo = TRUE;     
            }    
        }
    } 
    return colisaoDoInimigo; 
}


