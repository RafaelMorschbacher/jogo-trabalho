#include "bibjogo.h"

// FUNÇÕES GERAIS  ------------------------------------------------------------------------------------------------------------------
float alignCenterFont (char *v, int i, int fontSize, Font arcade) {
    float width;
    Vector2 tamText = MeasureTextEx(arcade, v, fontSize, 1);
    width = ((screenWidth-tamText.x)/2);
    return width;
}

// FUNÇOES NOVO JOGO
//cabeçalho
void desenhaCabecalho(PERSONAGEM *personagem, Texture2D iconeVidas, Font arcade, char *v) {
    DrawRectangle(0,0,screenWidth,50,GRAY);
    DrawTextEx(arcade, "Vidas ", (Vector2){100, 17}, 22, 1, LIGHTGRAY);
    int espacamento = 0;
    if((*personagem).vidas>0) {
        for(int i= (*personagem).vidas; i>0; i--) {
            DrawTexture(iconeVidas, (180 + espacamento), 10 , RAYWHITE);
            espacamento +=40;
        }
    }
    DrawTextEx(arcade, v, (Vector2){alignCenterFont(v,1,30,arcade), 12}, 30, 1, LIGHTGRAY);
}

// -- carregando mapa
int readLevel (FILE *level, float *positionX, float *positionY, char *tipo) { // função que le arquivo, 1 caractere por vez, e descobre qual é
    float xFuncao = *positionX;
    float yFuncao = *positionY;
    char conteudoTipo = *tipo;
    char leitura;

    do {
        fscanf(level,"%c",&leitura); // fiz a leitura do caractere

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

void checaColisaoArray(INIMIGO *inimigos, PERSONAGEM *personagem, OBSTACULO *obstaculos, int numObstaculos, Rectangle posicaoInicial, int nroInimigos) {

        for(int i=0; i<numObstaculos; i++) {
             if(!obstaculos[i].destruido)
                checaColisao(&(*personagem), &obstaculos[i].posicao, posicaoInicial);

        }
        for (int i=0;i<nroInimigos;i++) {
            checaColisao(&(*personagem), &(inimigos[i].posicao), posicaoInicial);
        }
}
int spawnParede(POWERUP *powerUp, OBSTACULO obstaculos[], int numObstaculos)
{
    int colisao = 0;
    for(int i=0; i<numObstaculos; i++)
    {
        if(CheckCollisionRecs(obstaculos[i].posicao, powerUp->posicao))
        {
            colisao = 1;
        }

    }

    return colisao;
}
void administraPowerUp(POWERUP *powerUp, PERSONAGEM *personagem, OBSTACULO *obstaculos, int numObstaculos, int larguraTela, int alturaTela)
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

void administraTiro(PERSONAGEM *personagem, int larguraTela, int alturaTela, OBSTACULO obstaculos[] ,int nroBlocos){
    //Atirando ao apertar SPACE
    if(IsKeyReleased(KEY_SPACE) && !personagem->tiro.atirando && personagem->tiro.numBalas>0){
        personagem->tiro.atirando = TRUE;
        personagem->tiro.posicao.x = personagem->posicao.x +10;
        personagem->tiro.posicao.y = personagem->posicao.y +10;
        personagem->tiro.inclinacao = personagem->inclinacao;
        personagem->tiro.numBalas -= 1;
        printf("%d",  personagem->tiro.numBalas);
    }

    //Funções após o disparo
    if(personagem->tiro.atirando){
        DrawRectangleRec(personagem->tiro.posicao, GREEN);
        //Animação de tiro
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
        //Tiro sai do cenário
        if(personagem->tiro.posicao.x > larguraTela || personagem->tiro.posicao.x < 0 || personagem->tiro.posicao.y >= alturaTela || personagem->tiro.posicao.y < 0){
            personagem->tiro.atirando = FALSE;
    }

    //Tiro atinge parede
        for(int i=0; i<nroBlocos; i++) {
            if(CheckCollisionRecs(personagem->tiro.posicao, obstaculos[i].posicao)){
                obstaculos[i].destruido = TRUE;
                personagem->tiro.atirando = FALSE;
            }
        }

    }



}

//FUNÇÕES ESPECIFICAS INIMIGOS
void criaInimigos(INIMIGO *inimigos, int nroInimigos, Texture inimigoTex, PERSONAGEM *personagem, OBSTACULO *obstaculo, int nroBlocos, char cor) {
    (inimigos[nroInimigos-1]).posicao.width = 25;
    (inimigos[nroInimigos-1]).posicao.height = 25;
    (inimigos[nroInimigos-1]).textura = inimigoTex;
    (inimigos[nroInimigos-1]).velocidade = VELOCIDADE_INIMIGO;
    (inimigos[nroInimigos-1]).orientacao = (nroInimigos % 4) * 90;
    do {
        (inimigos[nroInimigos-1]).posicao.x = (float)GetRandomValue(25, screenWidth-25);
        (inimigos[nroInimigos-1]).posicao.y = (float)GetRandomValue(75, 650-25);
    } while ((checaColisaoInimigos(nroInimigos, inimigos, personagem, (nroInimigos-1), &(*obstaculo), nroBlocos)) == TRUE); //para q nao seja criado em cima de onde n da

    (inimigos[nroInimigos-1]).modo = 'N';

    if (cor == 'G')
        (inimigos[nroInimigos-1]).cor = 'G';
    else if (cor == 'R')
        (inimigos[nroInimigos-1]).cor = 'R';
}
// FUNÇÃO P MODO (ALEATORIO OU PERSEGUIÇÃO) // OK
void modoInimigos (INIMIGO *inimigo, PERSONAGEM *personagem) {
    if ( ((fabs(inimigo->posicao.x) - ((*personagem).posicao.x)) < 7) || (fabs(inimigo->posicao.y - ((*personagem).posicao.y))) < 7  ){
            (inimigo->modo) = 'P';
    }
}

// to mandando &inimigos[i], ou seja, aqui to recebendo o endereço de uma struct inimigo
void movInimigos (INIMIGO *inimigo, Rectangle posicaoInicial, PERSONAGEM *personagem , int i, int colisaoInimigoCenario, int colisaoDoInimigo, char corInimigo, Texture inimigoRedUp, Texture inimigoRedDown, Texture inimigoRedLeft, Texture inimigoRedRight, Texture inimigoGreenUp, Texture inimigoGreenDown, Texture inimigoGreenLeft, Texture inimigoGreenRight) {
    float vp = 0; // serve pra mudar velocidade quandoe stiver em perseguição

    if ((inimigo->modo) == 'P') { //caso patrulha, muda sua orientação de acordo com a orientação do personagem
        vp = 2;
        if ((colisaoInimigoCenario == TRUE) || (colisaoDoInimigo == TRUE)) {
            vp = 0;
            (inimigo->modo) = 'N';
        }

        else {
            float diferencaPosicoesX = inimigo->posicao.x - (*personagem).posicao.x;
            float diferencaPosicoesY = inimigo->posicao.y - (*personagem).posicao.y;

            if ((fabs(inimigo->posicao.x - (*personagem).posicao.x) < 7 )) {
                if (diferencaPosicoesY >= 0)
                    inimigo->orientacao = 0;

                else if (diferencaPosicoesY < 0)
                    inimigo->orientacao = 180;
            }
            else if ((fabs(inimigo->posicao.y - (*personagem).posicao.y) < 7 )) {
                if (diferencaPosicoesX >= 0)
                    inimigo->orientacao = 270;
                else if (diferencaPosicoesX < 0)
                    inimigo->orientacao = 90;
            }
        }
    }

    if ( inimigo->modo == 'N') { //Normal (Patrulha)
        if (colisaoInimigoCenario == TRUE || colisaoDoInimigo == TRUE) {
            vp = 0;
            switch (inimigo->orientacao) {
                case 0: inimigo->posicao.y+= 5; break;
                case 90: inimigo->posicao.x-= 5; break;
                case 180: inimigo->posicao.y-= 5; break;
                case 270: inimigo->posicao.x+= 5; break;
            }
            inimigo->orientacao -= 90*(pow(-1,i));
        }
    }
        if (inimigo->orientacao == -90)
            inimigo->orientacao =270;
        if (inimigo->orientacao == 360)
            inimigo->orientacao = 0;

        switch(inimigo->orientacao) {
            case 0: {
                inimigo->posicao.y -= (float)VELOCIDADE_INIMIGO +vp;
                if (inimigo->cor == 'R')
                    inimigo->textura =  inimigoRedUp;
                if (inimigo->cor == 'G')
                    inimigo->textura =  inimigoGreenUp;
                break;
            }
            case 90: {
                inimigo->posicao.x+= (float)VELOCIDADE_INIMIGO + vp ;
                if (inimigo->cor == 'R')
                    inimigo->textura =  inimigoRedRight;
                if (inimigo->cor == 'G')
                    inimigo->textura =  inimigoGreenRight;
                break;
            }
            case 180: {
                inimigo->posicao.y += (float)VELOCIDADE_INIMIGO + vp ;
                if (inimigo->cor == 'R')
                    inimigo->textura =  inimigoRedDown;
                if (inimigo->cor == 'G')
                    inimigo->textura =  inimigoGreenDown;
                break;
            }
            case 270: {
                inimigo->posicao.x-= (float)VELOCIDADE_INIMIGO + vp;
                if (inimigo->cor == 'R')
                    inimigo->textura =  inimigoRedLeft;
                if (inimigo->cor == 'G')
                    inimigo->textura =  inimigoGreenLeft;
                break;
            }
        }
}


int checaColisaoInimigos(int numeroDeInimigos, INIMIGO *inimigos, PERSONAGEM *personagem, int numeroInimigo, OBSTACULO *obstaculo, int nroBlocos) { // VERFICAR ENDEREÇOS AQUI
    int colisaoDoInimigo = FALSE;

    for (int i = 0; i<nroBlocos; i++) {
        if ( CheckCollisionRecs(inimigos[numeroInimigo].posicao , (obstaculo[i].posicao)) ) {                 //checa colisao com tijolos
            colisaoDoInimigo = TRUE;
        }
        if ( CheckCollisionRecs(inimigos[numeroInimigo].posicao,(*personagem).posicao) ) {            //checa colisao com personagem
            colisaoDoInimigo = TRUE;
        }

    }
    for (int i = 0; i<numeroDeInimigos; i++) {
        if ( CheckCollisionRecs(inimigos[numeroInimigo].posicao, inimigos[i].posicao) ) {           //checa colisão com outros tanques
                if (numeroInimigo != i) {
                    colisaoDoInimigo = TRUE;
                }
        }
    }

    return colisaoDoInimigo;
}

