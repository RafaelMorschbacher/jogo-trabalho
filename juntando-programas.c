#include "bibjogo.h"
#include "bibjogo.c"

//VARIÁVEIS ------------------------------------------------------------------------------------------------------------------------
//variaveis menu
char menuOptions[3][10] =  {"Novo Jogo", "Continuar","Sair"};
float u;
char tipo = ' ';
int estadoChave = 1;
char fase[3][10] = {"FASE 1"," FASE 2", "FASE 3"};
//variaveis obstaculos
float positionX = 0;                  //coordenada x de um objeto na tela
float positionY = 1;                  //coordenada y de um objeto na tela
int nroBlocos = 0;
OBSTACULO obstaculos[600] ={0}; //posX, posY, largura, altura
//variaveis movimentação
int positionPlayer[1][2] = {0};
//variaveis inimigos
int nroInimigos = 0;
clock_t tempo[2];
int aux = 1;
char corInimigo;
int colisaoDoInimigo = FALSE;
int colisaoInimigoCenario = FALSE;
int maxInimigos =MAX_INIMIGOS;
int inimigosMortos =0;

//testando variaveis aqui
INIMIGO inimigos[MAX_INIMIGOS] = {}; //se chega no limite do tamanho bugaS

#define screenHeight 650

typedef enum gameScreen {MENU = 0, NOVOJOGO, CONTINUAR} gameScreen;

int main(void) {

    // Init------------------------------------------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "Battle INF - MENU");
    gameScreen currentScreen = MENU;


    //Arquivo

    FILE *fileLevel; // lembrar de colocar if else com problema pra abrir arquivo ou nao
    fileLevel = fopen("../levels/nivel1.txt", "r");

    // Load------------------------------------------------------------------------------------------------------------------------
    Image titulo = LoadImage("../assets/battleInf.png");
    Texture2D tituloTex = LoadTextureFromImage(titulo);
    Font arcade = LoadFont("../assets/ARCADECLASSIC.ttf");

    //texturas obstaculos

    Image brick = LoadImage("../assets/brick_texture2.png");
    ImageResize(&brick, 25, 40);
    Texture2D brickTexture = LoadTextureFromImage(brick);

    //texturas personagem
    Texture2D personagemUp = LoadTexture("../assets/personagem_up30x30.png");
    Texture2D personagemDown = LoadTexture("../assets/personagem_down30x30.png");
    Texture2D personagemLeft = LoadTexture("../assets/personagem_left30x30.png");
    Texture2D personagemRight = LoadTexture("../assets/personagem_right30x30.png");

    //celula de energia
    Texture2D energyCellTextura = LoadTexture("../assets/booster25x25.png");
    //escudo
    Image escudo = LoadImage("../assets/shield.png");
    ImageResize(&escudo, 40, 40);
    Texture2D escudoTextura = LoadTextureFromImage(escudo);

    //textura inimigo vermelho
    Texture2D inimigoRedUp = LoadTexture("../assets/inimigo-vermelho_up30x30.png");
    Texture2D inimigoRedDown = LoadTexture("../assets/inimigo-vermelho_down30x30.png");
    Texture2D inimigoRedLeft = LoadTexture("../assets/inimigo-vermelho_left30x30.png");
    Texture2D inimigoRedRight = LoadTexture("../assets/inimigo-vermelho_right30x30.png");
    Texture2D inimigoDead = LoadTexture("../assets/inimigo-destruido30x30.png");

    //textura inimigo verde
    Texture2D inimigoGreenUp = LoadTexture("../assets/inimigo-verde_up30x30.png");
    Texture2D inimigoGreenDown = LoadTexture("../assets/inimigo-verde_down30x30.png");
    Texture2D inimigoGreenLeft = LoadTexture("../assets/inimigo-verde_left30x30.png");
    Texture2D inimigoGreenRight = LoadTexture("../assets/inimigo-verde_right30x30.png");


    //Inicialização do Personagem
    PERSONAGEM personagem = {0};
    personagem.posicao.width = 25;
    personagem.posicao.height = 25;
    personagem.velocidadeBase = VELOCIDADE_INICIAL;
    personagem.velocidadeAtual = personagem.velocidadeBase;
    personagem.textura = personagemUp;
    personagem.vidas = 3;
    personagem.tiro.posicao.height = 10;
    personagem.tiro.posicao.width = 10;
    personagem.tiro.velocidade = 10;
    //personagem.tiro.numBalas = 3;

    //Inicializando PowerUp
    POWERUP powerUp = {0};
    powerUp.posicao.width = 25;
    powerUp.posicao.height = 25;
    powerUp.textura = energyCellTextura;
    do{
        powerUp.posicao.x = GetRandomValue(powerUp.posicao.width, GetScreenWidth()-powerUp.posicao.width);
        powerUp.posicao.y = GetRandomValue(powerUp.posicao.height+40,GetScreenHeight()- powerUp.posicao.height);
    }while(spawnParede(&powerUp, obstaculos, nroBlocos));

    //Inicializando Inimigos:
    //normal(patrulha) ou perseguição
    //aleatorio ou seguindo? ***ver se precisa pois já tem MODO.


    SetTargetFPS(60);
    while (!WindowShouldClose()) { // detecta se vai fechar
        // Update
        switch (currentScreen) {
            case MENU:{
                if (IsKeyPressed(KEY_DOWN))
                        estadoChave++;
                if (IsKeyPressed(KEY_UP))
                        estadoChave--;
                if (estadoChave == 4)
                        estadoChave = 1;
                if (estadoChave == 0)
                        estadoChave = 3;
                if (estadoChave == 1 && IsKeyPressed(KEY_ENTER))
                        currentScreen = NOVOJOGO;
                if (estadoChave == 2 && IsKeyPressed(KEY_ENTER))
                        currentScreen = CONTINUAR;
                break;
            }

            case NOVOJOGO: {
                //update dos obstaculos (tijolos)
                while (readLevel(fileLevel, &positionX, &positionY,&tipo) == 0) { // enquanto tiver coisas para ler
                    if (tipo == '#') { // se a função parou num #, desenha o bloco
                        //adicionando as características pro array de structs obstaculos
                        obstaculos[nroBlocos].posicao.x = (positionX-1)*25;
                        obstaculos[nroBlocos].posicao.y = ((positionY-1)*40)+50;
                        obstaculos[nroBlocos].posicao.width = 25.0;
                        obstaculos[nroBlocos].posicao.height = 40.0;
                        nroBlocos++;
                    }
                    else if (tipo == 'T') { // se parou num T, encontra a posição do jogador
                        personagem.posicao.x = (positionX-1)*25;
                        personagem.posicao.y = ((positionY-1)*40)+50;
                    }
                }

                //Update dos inimigos
                //---- criar novos inimigos a cada 5seg
                tempo[1] = clock();
                if (aux == 1) {                         // para entrar pela primeira vez no while
                    tempo[0] = tempo[1] - 5000;
                    aux--;
                }

                int tempoPassado = ((tempo[1]-tempo[0])/1000);

                if ((nroInimigos % 2) == 0)             // escolher cor dos inimigos
                    corInimigo = 'R';
                else if ((nroInimigos % 2) == 1)
                    corInimigo = 'G';

                if (tempoPassado == 5) {
                    if (nroInimigos < maxInimigos) {
                        nroInimigos++; //aumenta os inimigos q vão aparecendo
                        if (corInimigo == 'R')
                            criaInimigos(inimigos, nroInimigos, inimigoRedDown, &personagem, obstaculos, nroBlocos, corInimigo); //adicionar corretamente //mando o array de structs
                        else if (corInimigo == 'G')
                            criaInimigos(inimigos, nroInimigos, inimigoGreenDown, &personagem, obstaculos, nroBlocos, corInimigo);
                        tempo[0] = clock();
                    }
                }

                //---- faze-los andar

                for (int i = 0; i < nroInimigos; i++) { // vai um a um nos inimigos, até o último (nroInimigos)
                    if(inimigos[i].vivo == TRUE){
                        Rectangle posicaoInicialInimigo = inimigos[i].posicao;
                        colisaoInimigoCenario = FALSE;
                        colisaoDoInimigo = FALSE;
                        bool ultrapassaCenario = (inimigos[i].posicao.x > screenWidth - inimigos[i].posicao.width) || (inimigos[i].posicao.x < 0) || (inimigos[i].posicao.y > screenHeight - inimigos[i].posicao.height) || (inimigos[i].posicao.y < 50);
                        if(ultrapassaCenario) {
                            inimigos[i].posicao = posicaoInicialInimigo;
                            colisaoInimigoCenario = TRUE; // manda a informação de q colidiu com algo (nesse caso, extremos da janela)
                        }
                        colisaoDoInimigo = checaColisaoInimigos(nroInimigos, inimigos, &personagem, i, obstaculos, nroBlocos);
                        if(colisaoDoInimigo)
                            inimigos[i].posicao = posicaoInicialInimigo;

                        modoInimigos(&inimigos[i], &personagem); //mando o endereço de um inimigo em especifico
                        movInimigos (&inimigos[i], posicaoInicialInimigo, &personagem, i, colisaoInimigoCenario, colisaoDoInimigo, corInimigo, inimigoRedUp,  inimigoRedDown,  inimigoRedLeft,  inimigoRedRight,  inimigoGreenUp,  inimigoGreenDown,  inimigoGreenLeft,  inimigoGreenRight);
                        //faz a movimentação já levando em consideração o modo

                        //Tiro dos inimigos
                        administraTiroInimigos(&inimigos[i]);
                        }

                }

                //Update da posicao e textura do personagem
                Rectangle posicaoInicial = personagem.posicao;// Guardando posicao inicial antes de colisoes, etc
                atualizaPosicao(&personagem, personagemRight, personagemLeft, personagemUp, personagemDown);
                //Colisao Cenario
                bool ultrapassaCenario = (personagem.posicao.x > screenWidth - personagem.posicao.width) || (personagem.posicao.x <0) || (personagem.posicao.y > screenHeight - personagem.posicao.height) || (personagem.posicao.y <50);
                if(ultrapassaCenario)
                    personagem.posicao = posicaoInicial;
                //Colisao Obstaculos
                checaColisaoArray(inimigos, &personagem, obstaculos, nroBlocos,posicaoInicial,nroInimigos);
                break;
            }

            case CONTINUAR: {
                break;
            }
        }

        // Draw
        BeginDrawing();
            ClearBackground(RAYWHITE);
            switch (currentScreen) {
                case MENU: {
                    DrawRectangle(0, 0, screenWidth, screenHeight, BLACK);
                    DrawTexture(tituloTex, 0, 20 , WHITE);
                    u = 0; //escreve menu
                    for (int i = 0; i < 3; i++) {
                        DrawTextEx(arcade, menuOptions[i], (Vector2){(alignCenterFont(menuOptions[i],i,40, arcade)),(250 + u) }, 40, 1,GRAY);
                        u+= 50.0;
                    }
                    switch (estadoChave) {//deixa amarelo
                        case 1: DrawTextEx(arcade, menuOptions[0], (Vector2){(alignCenterFont(menuOptions[0],0,40, arcade)),250 }, 40, 1,YELLOW);
                            break;
                        case 2: DrawTextEx(arcade, menuOptions[1], (Vector2){(alignCenterFont(menuOptions[1],1,40, arcade)),(300) }, 40, 1,YELLOW);
                            break;
                        case 3: DrawTextEx(arcade, menuOptions[2], (Vector2){(alignCenterFont(menuOptions[2],2,40, arcade)),(350) }, 40, 1,YELLOW);
                            break;
                        default: break;
                    }

                    break;
                }

                case NOVOJOGO: {
                    ClearBackground(RAYWHITE);
                    DrawRectangle(0,0,screenWidth,screenHeight,BLACK);

                    for (int j = 0; j < nroBlocos; j++ ) {
                        //Desenhar apenas Blocos nao destruidos
                        if(!obstaculos[j].destruido){
                            int xvec = obstaculos[j].posicao.x;
                            int yvec =  obstaculos[j].posicao.y;
                            DrawTexture(brickTexture, xvec, yvec, WHITE);
                        }

                    }
                    //DrawRectangleRec

                    administraPowerUp(&powerUp, &personagem, obstaculos, nroBlocos, screenHeight, screenWidth);
                    administraTiro(&personagem, screenWidth, screenHeight, obstaculos, nroBlocos, inimigos, nroInimigos, &maxInimigos, &inimigosMortos);
                    DrawTexture(personagem.textura, (personagem.posicao.x ), (personagem.posicao.y ), RAYWHITE);
                    desenhaCabecalho(&personagem, escudoTextura, arcade, fase[0]);

                    //desenhando inimigos na tela
                    for (int i = 0; i<nroInimigos; i++) {
                        if(inimigos[i].vivo==TRUE)
                            DrawTexture(inimigos[i].textura, (inimigos[i].posicao.x),  (inimigos[i].posicao.y), RAYWHITE);
                        if(inimigos[i].vivo==FALSE)
                            DrawTexture(inimigoDead, (inimigos[i].posicao.x),  (inimigos[i].posicao.y), RAYWHITE);


                    }
                    break;

                }

                case CONTINUAR:{
                    DrawRectangle(0, 0, screenWidth, screenHeight, BLACK);
                    DrawTextEx(arcade, "Continuar", (Vector2){200,350}, 75, 1,WHITE);
                    break;
                }
                default: break;
            }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    //Unload
    UnloadTexture(tituloTex);
    fclose (fileLevel);

    CloseWindow();
    return 0;
}

