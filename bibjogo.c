#include "bibjogo.h"

// FUNÇÕES GERAIS  ------------------------------------------------------------------------------------------------------------------
float alignCenterFont (char *v, int i, int fontSize, Font arcade) {
    float width;
    Vector2 tamText = MeasureTextEx(arcade, v, fontSize, 1);
    width = ((screenWidth-tamText.x)/2);
    return width;
}


// FUNÇOES NOVO JOGO -----------------------------------------------------------------------------------------------------------------

//cabeçalho
void desenhaCabecalho(PERSONAGEM *personagem, Texture2D iconeVidas, Font arcade, char *v) {

    char pontos[10] = {};
  

    switch (personagem->fase) {
        case 1: sprintf(pontos,"%i",personagem->pontos[0]); break;
        case 2: sprintf(pontos,"%i",personagem->pontos[1]); break;
        case 3: sprintf(pontos,"%i",personagem->pontos[2]); break;
        case 4: sprintf(pontos,"%i",personagem->pontos[3]); break;
    }
    
    DrawRectangle(0,0,screenWidth,50,GRAY);

    DrawTextEx(arcade, "Vidas ", (Vector2){100, 17}, 22, 1, LIGHTGRAY);

    int espacamento = 0;
    if((*personagem).vidas>0) {
        for(int i= (*personagem).vidas; i>0; i--) {
            DrawTexture(iconeVidas, (180 + espacamento), 10 , RAYWHITE);
            espacamento +=40;
        }
    }

    DrawTextEx(arcade, "Pontos ", (Vector2){850, 17}, 22, 1, LIGHTGRAY);
    DrawText(pontos, 935, 17, 22, LIGHTGRAY);

    DrawTextEx(arcade, personagem->nome, (Vector2){alignCenterFont(personagem->nome,1,20,arcade), 27}, 20, 1, LIGHTGRAY);

    DrawTextEx(arcade, v, (Vector2){alignCenterFont(v,1,30,arcade), 6}, 30, 1, LIGHTGRAY);
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

//FUNCOES FASES ---------------------------------------------------------------------------------------------------------------------------
//criar funções para encurtar as fases

void criandoMapa(FILE *fileLevel, float *positionX, float *positionY, char *tipo, OBSTACULO obstaculos[], int *nroBlocos, PERSONAGEM *personagem) {

    while (readLevel(fileLevel, positionX, positionY,tipo) == 0) { // enquanto tiver coisas para ler
        if (*tipo == '#') { // se a função parou num #, desenha o bloco
            //adicionando as características pro array de structs obstaculos
            obstaculos[*nroBlocos].posicao.x = ((*positionX)-1)*25;
            obstaculos[*nroBlocos].posicao.y = (((*positionY)-1)*40)+50;
            obstaculos[*nroBlocos].posicao.width = 25.0;
            obstaculos[*nroBlocos].posicao.height = 40.0;
            obstaculos[*nroBlocos].destruido = FALSE; 
            (*nroBlocos)++;
        }
        else if (*tipo == 'T') { // se parou num T, encontra a posição do jogador
            personagem->posicao.x = ((*positionX)-1)*25;
            personagem->posicao.y = (((*positionY)-1)*40)+50;
        }
    }
}

void criandoInimigos(int *inimigosEmTela, clock_t tempo[], int *aux, int *nroInimigos, int *nroBlocos, char *corInimigo, INIMIGO inimigos[], Texture inimigoRedDown, Texture inimigoGreenDown, OBSTACULO obstaculos[], PERSONAGEM *personagem) {

    tempo[1] = clock();
    if ((*aux) == 1) {                         // para entrar pela primeira vez no while
        tempo[0] = tempo[1] - 5000;
        (*aux)--;
    }
    int tempoPassado = ((tempo[1]-tempo[0])/1000);


    if (((*nroInimigos) % 2) == 0)             // escolher cor dos inimigos
            (*corInimigo) = 'R';
    else if (((*nroInimigos) % 2) == 1)
            (*corInimigo) = 'G';

    if (tempoPassado == 5) {
        if ((*nroInimigos) < MAX_INIMIGOS) {
            (*inimigosEmTela)++; 
           (*nroInimigos)++; //aumenta os inimigos q vão aparecendo
            if ((*corInimigo) == 'R')
                criaInimigos(inimigos, *nroInimigos, inimigoRedDown, personagem, obstaculos, *nroBlocos, *corInimigo); //adicionar corretamente //mando o array de structs
            else if ((*corInimigo) == 'G')
                criaInimigos(inimigos, *nroInimigos, inimigoGreenDown, personagem, obstaculos, *nroBlocos, *corInimigo);
            tempo[0] = clock();
        }
    }
}

void movendoInimigos (int screenHeight, int *nroInimigos, int *nroBlocos, int *colisaoInimigoCenario, int *colisaoDoInimigo, INIMIGO inimigos[], PERSONAGEM *personagem, OBSTACULO obstaculos[], char *corInimigo, Texture inimigoRedUp, Texture inimigoRedDown, Texture inimigoRedLeft, Texture inimigoRedRight, Texture inimigoGreenUp, Texture inimigoGreenDown, Texture inimigoGreenLeft, Texture inimigoGreenRight) {
    for (int i = 0; i < (*nroInimigos); i++) { // vai um a um nos inimigos, até o último (nroInimigos)

        Rectangle posicaoInicialInimigo = inimigos[i].posicao;
        (*colisaoInimigoCenario) = FALSE;
        (*colisaoDoInimigo) = FALSE;

        bool ultrapassaCenario = (inimigos[i].posicao.x > screenWidth - inimigos[i].posicao.width) || (inimigos[i].posicao.x < 0) || (inimigos[i].posicao.y > screenHeight - inimigos[i].posicao.height) || (inimigos[i].posicao.y < 50);
        if(ultrapassaCenario) {
            inimigos[i].posicao = posicaoInicialInimigo;
            (*colisaoInimigoCenario) = TRUE; // manda a informação de q colidiu com algo (nesse caso, extremos da janela)
        }

        inimigos[i].colisao = FALSE; //variavel que será usada para colisão entre inimigos
        (*colisaoDoInimigo) = checaColisaoInimigos((*nroInimigos), inimigos, personagem, i, obstaculos, (*nroBlocos));
        if((*colisaoDoInimigo))
            inimigos[i].posicao = posicaoInicialInimigo;

        modoInimigos(&inimigos[i], personagem); //mando o endereço de um inimigo em especifico
        if(inimigos[i].vivo)
            movInimigos (&inimigos[i], posicaoInicialInimigo, personagem, i, (*colisaoInimigoCenario), (*colisaoDoInimigo), (*corInimigo), inimigoRedUp,  inimigoRedDown,  inimigoRedLeft,  inimigoRedRight,  inimigoGreenUp,  inimigoGreenDown,  inimigoGreenLeft,  inimigoGreenRight);
        //faz a movimentação já levando em consideração o modo


      //  administraTiroInimigos( &inimigos[i], screenWidth, screenHeight, obstaculos, *nroBlocos, personagem); //Tiro dos inimigos


    }
}

void movendoPersonagem (PERSONAGEM *personagem,int *nroBlocos, int *nroInimigos, int screenHeight, INIMIGO inimigos[], OBSTACULO obstaculos[], Texture personagemRight, Texture personagemLeft, Texture personagemUp, Texture personagemDown) {
    Rectangle posicaoInicial = personagem->posicao;// Guardando posicao inicial antes de colisoes, etc
    atualizaPosicao(personagem, personagemRight, personagemLeft, personagemUp, personagemDown);
    //Colisao Cenario
    bool ultrapassaCenario = (personagem->posicao.x > screenWidth - personagem->posicao.width) || (personagem->posicao.x <0) || (personagem->posicao.y > screenHeight - personagem->posicao.height) || (personagem->posicao.y <50);
    if(ultrapassaCenario)
        personagem->posicao = posicaoInicial;
    //Colisao Obstaculos
    checaColisaoArray(inimigos, personagem, obstaculos, (*nroBlocos), posicaoInicial, (*nroInimigos));
}


//FUNÇÕES CONTINUAR ---------------------------------------------------------------------------------------------------------------------

//continuar fazendo essa função
void salvarJogo(PERSONAGEM *personagem, INIMIGO inimigos[], OBSTACULO obstaculos[], int *nroInimigos, int *nroObstaculos, int *inimigosMortos, int *inimigosEmTela, int *maxInimigos) {
    FILE *savePointer;
    savePointer = fopen("../levels/continuar.bin", "wb");

    if (savePointer != NULL){
        fwrite (personagem, sizeof(PERSONAGEM), 1, savePointer);

        fwrite (inimigosMortos, sizeof(int), 1, savePointer);
        fwrite (inimigosEmTela, sizeof(int), 1, savePointer);
        fwrite (maxInimigos, sizeof(int), 1, savePointer);

        fwrite (nroInimigos, sizeof(int), 1, savePointer);
        fwrite (nroObstaculos, sizeof(int), 1, savePointer);

        for (int i = 0; i < (*nroInimigos); i++) {
            fwrite (&inimigos[i], sizeof(INIMIGO), 1, savePointer);
        }

        for (int i = 0; i < (*nroObstaculos); i++) {
            fwrite (&obstaculos[i], sizeof(OBSTACULO), 1, savePointer);
        }
    }
    fclose(savePointer);
}


void lerJogo(PERSONAGEM *personagem, INIMIGO inimigos[], OBSTACULO obstaculos[], int *nroInimigos, int *nroObstaculos, int *inimigosMortos, int *inimigosEmTela, int *maxInimigos) {
    FILE *savePointer;
    savePointer = fopen("../levels/continuar.bin", "rb");

    if (savePointer != NULL){
        fread (personagem, sizeof(PERSONAGEM), 1, savePointer);     //lendo personagem
       
        fread (inimigosMortos, sizeof(int), 1, savePointer);        //lendo numero de inimigos mortos
        fread (inimigosEmTela, sizeof(int), 1, savePointer);        //lendo numero de inimigos em tela
        fread (maxInimigos, sizeof(int), 1, savePointer);

        fread (nroInimigos, sizeof(int), 1, savePointer);           //lendo numero de inimigos
        fread (nroObstaculos, sizeof(int), 1, savePointer);         //lendo numero de obstaculos

        for (int i = 0; i < (*nroInimigos); i++) {
            fread(&inimigos[i], sizeof(INIMIGO), 1, savePointer);       //lendo STRUCT inimigos um a um
        }

        for (int i = 0; i < (*nroObstaculos); i++) {
            fread(&obstaculos[i], sizeof(OBSTACULO), 1, savePointer);   //lendo STRUCT obstaculos um a um 
        }
    }
    fclose(savePointer);
}

//FUNCAO SCORE -------------------------------------------------------------------------------------------------------------------------
void salvaScore (HIGHSCORE recordistas[], int recorde, PERSONAGEM *personagem){
   
    FILE *scoresPointer;
    scoresPointer = fopen("../levels/highscores.bin", "rb");


    int indice = 6; 
   

    for (int i = 0; i < 5; i++) {
        fread(&recordistas[i], sizeof(HIGHSCORE), 1, scoresPointer);      //le o arquivo binario para struct
    }

    for (int i = 4; i >= 0; i--) {
        if (recorde > recordistas[i].pontos) {                          //passa pelas structs e salva o ultimo valor q o recorde é maior
            indice = i;                                                 //(de 0, maior, até 4, menor)
        }
    }
    if (indice != 6) {
        for (int i = 4; i < indice; i--){                               //reorganiza os recordes
            recordistas[i] = recordistas[i-1]; 
        }
        recordistas[indice].pontos = recorde;                           //substitui o nome recorde em sua posição
        strcpy (recordistas[indice].nome, personagem->nome); 
    }
    fclose(scoresPointer); 

    scoresPointer = fopen("../levels/highscores.bin", "wb");

    for (int i = 0; i < 5; i++) {
        fwrite(&recordistas[i], sizeof(HIGHSCORE), 1, scoresPointer);      //escreve structs de volta no arquivo
    }
    fclose(scoresPointer); 

}

            
// FUNCOES MOVIMENTAÇÃO & power UP ------------------------------------------------------------------------------------------------------
void checaColisao(PERSONAGEM *personagem, Rectangle obstaculo, Rectangle posicaoInicial) {
    if(CheckCollisionRecs(personagem->posicao, obstaculo)) {
        personagem->posicao = posicaoInicial;
    }
}


/*
void checaColisaoAux(PERSONAGEM *personagem, Rectangle *inimigo, Rectangle posicaoInicial) { // criei
    if(CheckCollisionRecs(personagem->posicao,*inimigo)) {
        personagem->posicao = posicaoInicial;
    }
}
*/

void checaColisaoArray(INIMIGO *inimigos, PERSONAGEM *personagem, OBSTACULO obstaculos[], int numObstaculos, Rectangle posicaoInicial, int nroInimigos) {

        for(int i=0; i<numObstaculos; i++) {
             if(!obstaculos[i].destruido) {
                checaColisao(&(*personagem), obstaculos[i].posicao, posicaoInicial);
             }

        }
        for (int i=0;i<nroInimigos;i++) {
            checaColisao(&(*personagem), inimigos[i].posicao, posicaoInicial);
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

void administraTiro( PERSONAGEM *personagem, int larguraTela, int alturaTela, OBSTACULO obstaculos[] ,int nroBlocos, INIMIGO inimigos[], int nroInimigos, int *maxInimigos, int *inimigosMortos, int *inimigosEmTela){
  
    //Atirando ao apertar SPACE
    if(IsKeyReleased(KEY_SPACE) && !personagem->tiro.atirando){
        personagem->tiro.atirando = TRUE;
        personagem->tiro.posicao.x = personagem->posicao.x +10;
        personagem->tiro.posicao.y = personagem->posicao.y +10;
        personagem->tiro.inclinacao = personagem->inclinacao;
        //personagem->tiro.numBalas -= 1;
    }

    //Funções após o disparo
    if(personagem->tiro.atirando){
        DrawRectangleRec(personagem->tiro.posicao, BLUE);
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
        //Tiro sai do cenário : destruir tiro
        if(personagem->tiro.posicao.x > larguraTela || personagem->tiro.posicao.x < 0 || personagem->tiro.posicao.y >= alturaTela || personagem->tiro.posicao.y < 40){
            personagem->tiro.atirando = FALSE;
    }

    //Tiro atinge parede
        for(int i=0; i<nroBlocos; i++) {
            if(CheckCollisionRecs(personagem->tiro.posicao, obstaculos[i].posicao)&& !obstaculos[i].destruido){
                obstaculos[i].destruido = TRUE;
                personagem->tiro.atirando = FALSE;

                switch (personagem->fase) {
                    case 1: personagem->pontos[0] -= 50; break;
                    case 2: personagem->pontos[1] -= 50; break;
                    case 3: personagem->pontos[2] -= 50; break;
                    case 4: personagem->pontos[3] -= 50; break;
                }
                
                 
            }
        }
       

    //Tiro atinge inimigo
        for(int i=0; i<nroInimigos; i++){
            if(CheckCollisionRecs(personagem->tiro.posicao, inimigos[i].posicao) && inimigos[i].vivo == 1){
                inimigos[i].vivo = FALSE;
                inimigos[i].contadorAnimacao = 0;
                personagem->tiro.atirando = FALSE;
                *maxInimigos +=1;
                *inimigosMortos += 1;
                *inimigosEmTela -=1;
                switch (personagem->fase) {
                    case 1: personagem->pontos[0] += 800; break;
                    case 2: personagem->pontos[1] += 800; break;
                    case 3: personagem->pontos[2] += 800; break;
                    case 4: personagem->pontos[3] += 800; break;
                } 
            }
        }
    }


}


//FUNÇÕES ESPECIFICAS INIMIGOS
void criaInimigos(INIMIGO *inimigos, int nroInimigos, Texture inimigoTex, PERSONAGEM *personagem, OBSTACULO obstaculo[], int nroBlocos, char cor) {
    (inimigos[nroInimigos-1]).posicao.width = 25;
    (inimigos[nroInimigos-1]).posicao.height = 25;
    (inimigos[nroInimigos-1]).textura = inimigoTex;
    (inimigos[nroInimigos-1]).velocidade = VELOCIDADE_INIMIGO;
    (inimigos[nroInimigos-1]).orientacao = (nroInimigos % 4) * 90;
    (inimigos[nroInimigos-1]).vivo = TRUE;
    do {
        (inimigos[nroInimigos-1]).posicao.x = (float)GetRandomValue(25, screenWidth-25);
        (inimigos[nroInimigos-1]).posicao.y = (float)GetRandomValue(75, 650-25);
    } while ((checaColisaoInimigos(nroInimigos, inimigos, personagem, (nroInimigos-1), obstaculo, nroBlocos)) == TRUE); //para q nao seja criado em cima de onde n da

    (inimigos[nroInimigos-1]).modo = 'N';

    if (cor == 'G')
        (inimigos[nroInimigos-1]).cor = 'G';
    else if (cor == 'R')
        (inimigos[nroInimigos-1]).cor = 'R';
}
// FUNÇÃO P MODO (ALEATORIO OU PERSEGUIÇÃO) // OK
void modoInimigos (INIMIGO *inimigo, PERSONAGEM *personagem) {

    if ( (fabs(inimigo->posicao.x - (*personagem).posicao.x) < 7) || (fabs(inimigo->posicao.y - (*personagem).posicao.y) < 7 ) ){
            (inimigo->modo) = 'P';

    }
}

// to mandando &inimigos[i], ou seja, aqui to recebendo o endereço de uma struct inimigo
void movInimigos (INIMIGO *inimigo, Rectangle posicaoInicial, PERSONAGEM *personagem , int i, int colisaoInimigoCenario, int colisaoDoInimigo, char corInimigo, Texture inimigoRedUp, Texture inimigoRedDown, Texture inimigoRedLeft, Texture inimigoRedRight, Texture inimigoGreenUp, Texture inimigoGreenDown, Texture inimigoGreenLeft, Texture inimigoGreenRight) {

    if ((inimigo->modo) == 'P') { //caso patrulha, muda sua orientação de acordo com a orientação do personagem

        inimigo->velocidade = ((float)VELOCIDADE_INIMIGO + 2.0);

        if ((colisaoInimigoCenario == TRUE) || (colisaoDoInimigo == TRUE)) {

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


        if ( inimigo->modo == 'N') {
             inimigo->velocidade =VELOCIDADE_INIMIGO;
            if (colisaoInimigoCenario == TRUE || colisaoDoInimigo == TRUE) {
                switch (inimigo->orientacao) {
                    case 0:   inimigo->posicao.y+= 5; break;
                    case 90:  inimigo->posicao.x-= 5; break;
                    case 180: inimigo->posicao.y-= 5; break;
                    case 270: inimigo->posicao.x+= 5; break;
                }
                inimigo->orientacao -= 90*(pow(-1,i));
            }

        }

            if (inimigo->orientacao == -90)
                inimigo->orientacao = 270;
            if (inimigo->orientacao == 360)
                inimigo->orientacao = 0;

            switch(inimigo->orientacao) {
                case 0: {
                //  inimigo->posicao.y -= VELOCIDADE_INIMIGO ; //era assim, vou alterar para testes
                    inimigo->posicao.y -=  inimigo->velocidade;
                    if (inimigo->cor == 'R')
                        inimigo->textura =  inimigoRedUp;
                    if (inimigo->cor == 'G')
                        inimigo->textura =  inimigoGreenUp;
                    break;
                }
                case 90: {
                // inimigo->posicao.x+= VELOCIDADE_INIMIGO;
                inimigo->posicao.x +=  inimigo->velocidade;
                    if (inimigo->cor == 'R')
                        inimigo->textura =  inimigoRedRight;
                    if (inimigo->cor == 'G')
                        inimigo->textura =  inimigoGreenRight;
                    break;
                }
                case 180: {
                // inimigo->posicao.y += VELOCIDADE_INIMIGO;
                inimigo->posicao.y +=  inimigo->velocidade;
                    if (inimigo->cor == 'R')
                        inimigo->textura =  inimigoRedDown;
                    if (inimigo->cor == 'G')
                        inimigo->textura =  inimigoGreenDown;
                    break;
                }
                case 270: {
                // inimigo->posicao.x-= VELOCIDADE_INIMIGO;
                inimigo->posicao.x-=inimigo->velocidade;
                    if (inimigo->cor == 'R')
                        inimigo->textura =  inimigoRedLeft;
                    if (inimigo->cor == 'G')
                        inimigo->textura =  inimigoGreenLeft;
                    break;
                }
            }
    }


int checaColisaoInimigos(int numeroDeInimigos, INIMIGO inimigos[], PERSONAGEM *personagem, int numeroInimigo, OBSTACULO obstaculo[], int nroBlocos) { // VERFICAR ENDEREÇOS AQUI
    int colisaoDoInimigo = FALSE;

    for (int i = 0; i<nroBlocos; i++) {
        if ( CheckCollisionRecs(inimigos[numeroInimigo].posicao , (obstaculo[i].posicao)) && inimigos[numeroInimigo].vivo==TRUE && !obstaculo[i].destruido ) {                 //checa colisao com tijolos
            colisaoDoInimigo = TRUE;
        }
        if ( CheckCollisionRecs(inimigos[numeroInimigo].posicao,(*personagem).posicao) && inimigos[numeroInimigo].vivo==TRUE ) {            //checa colisao com personagem
            colisaoDoInimigo = TRUE;

        }
    }

    for (int i = 0; i<numeroDeInimigos; i++) {

        if ( CheckCollisionRecs(inimigos[numeroInimigo].posicao, inimigos[i].posicao) && inimigos[numeroInimigo].vivo==TRUE ) {           //checa colisão com outros tanques
                if (numeroInimigo != i) {
                    colisaoDoInimigo = TRUE;
                }
        }
    }

    return colisaoDoInimigo;
}

void administraTiroInimigos(INIMIGO *inimigo, int larguraTela, int alturaTela, OBSTACULO obstaculos[], int nroBlocos, PERSONAGEM *personagem){
    //Tiro aleatório se não estiver já atirando
    if(!GetRandomValue(0,20) && !inimigo->tiro.atirando && inimigo->vivo == 1){
        inimigo->tiro.atirando = TRUE;
        inimigo->tiro.posicao.x = inimigo->posicao.x +10;
        inimigo->tiro.posicao.y = inimigo->posicao.y +10;
        inimigo->tiro.posicao.height = 10;
        inimigo->tiro.posicao.width = 10;
        //Inclinação do tiro com correção (devido a referência de angulo diferente)
        switch(inimigo->orientacao){
            case 0: inimigo->tiro.inclinacao = 90;
            break;
            case 90: inimigo->tiro.inclinacao = 0;
            break;
            case 180: inimigo->tiro.inclinacao = 270;
            break;
            case 270: inimigo->tiro.inclinacao = 180;
        }


    }

    if(inimigo->tiro.atirando ==TRUE){
        DrawRectangleRec(inimigo->tiro.posicao, RED);
        switch(inimigo->tiro.inclinacao){
            case 0:
                inimigo->tiro.posicao.x += 10;//inimigo->tiro.velocidade;
            break;
            case 90:
                inimigo->tiro.posicao.y -= 10;//inimigo->tiro.velocidade;
            break;
            case 180:
                inimigo->tiro.posicao.x -= 10;//inimigo->tiro.velocidade;
            break;
            case 270:
                inimigo->tiro.posicao.y += 10;//inimigo->tiro.velocidade;
            break;
        }

        //Tiro sai do cenário : destruir tiro
        if(inimigo->tiro.posicao.x > larguraTela || inimigo->tiro.posicao.x < 0 || inimigo->tiro.posicao.y >= alturaTela || inimigo->tiro.posicao.y < 40){
            inimigo->tiro.atirando = FALSE;

        }


        //Tiro atinge parede
        for(int i=0; i<nroBlocos; i++) {
            if(CheckCollisionRecs(inimigo->tiro.posicao, obstaculos[i].posicao)&& !obstaculos[i].destruido){
                obstaculos[i].destruido = TRUE;
                inimigo->tiro.atirando = FALSE;

            }
        }

        //Tiro atinge personagem
        if(CheckCollisionRecs(inimigo->tiro.posicao , personagem->posicao)){
            personagem->vidas--;
            inimigo->tiro.atirando =0;
        }
    }

}

void explodeInimigo(INIMIGO *inimigo,Texture frame1,Texture frame2,Texture frame3,Texture frame4,Texture frame5,Texture frame6){
    int numFrames = 5;

                                if(inimigo->contadorAnimacao <= numFrames)
                                    DrawTexture(inimigo->textura, (inimigo->posicao.x),  (inimigo->posicao.y), RAYWHITE);
                                else if(inimigo->contadorAnimacao <= numFrames*2)
                                    DrawTexture(frame1, (inimigo->posicao.x), (inimigo->posicao.y), RAYWHITE);
                                else if(inimigo->contadorAnimacao <= numFrames*3)
                                    DrawTexture(frame2, (inimigo->posicao.x),  (inimigo->posicao.y), RAYWHITE);
                                else if(inimigo->contadorAnimacao <= numFrames*4)
                                    DrawTexture(frame3, (inimigo->posicao.x),  (inimigo->posicao.y), RAYWHITE);
                                else if(inimigo->contadorAnimacao <= numFrames*5)
                                    DrawTexture(frame4, (inimigo->posicao.x),  (inimigo->posicao.y), RAYWHITE);
                                else if(inimigo->contadorAnimacao <= numFrames*6)
                                    DrawTexture(frame5, (inimigo->posicao.x),  (inimigo->posicao.y), RAYWHITE);
                                else if(inimigo->contadorAnimacao <= numFrames*7)
                                    DrawTexture(frame6, (inimigo->posicao.x),  (inimigo->posicao.y), RAYWHITE);


                                if(inimigo->contadorAnimacao<= numFrames*7)
                                    inimigo->contadorAnimacao += 1;

}

void explodeObstaculo(OBSTACULO *obstaculo,Texture frame1,Texture frame2,Texture frame3,Texture frame4,Texture frame5,Texture frame6, Texture frame7){
    int numFrames = 5;

    if(obstaculo->contadorAnimacao <= numFrames)
        DrawTexture(frame1, (obstaculo->posicao.x),  (obstaculo->posicao.y), RAYWHITE);
    else if(obstaculo->contadorAnimacao <= numFrames*2)
        DrawTexture(frame2, (obstaculo->posicao.x), (obstaculo->posicao.y), RAYWHITE);
    else if(obstaculo->contadorAnimacao <= numFrames*3)
        DrawTexture(frame3, (obstaculo->posicao.x), (obstaculo->posicao.y), RAYWHITE);
    else if(obstaculo->contadorAnimacao <= numFrames*4)
        DrawTexture(frame4, (obstaculo->posicao.x), (obstaculo->posicao.y), RAYWHITE);
    else if(obstaculo->contadorAnimacao <= numFrames*5)
        DrawTexture(frame5, (obstaculo->posicao.x), (obstaculo->posicao.y), RAYWHITE);
    else if(obstaculo->contadorAnimacao <= numFrames*6)
        DrawTexture(frame6, (obstaculo->posicao.x), (obstaculo->posicao.y), RAYWHITE);
    else if(obstaculo->contadorAnimacao <= numFrames*7)
        DrawTexture(frame7, (obstaculo->posicao.x), (obstaculo->posicao.y), RAYWHITE);


    if(obstaculo->contadorAnimacao<= numFrames*7)
        obstaculo->contadorAnimacao += 1;

}
