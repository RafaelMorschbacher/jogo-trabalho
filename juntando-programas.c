#include <stdio.h>
#include <raylib.h>
#include <string.h>
#include "bibjogo.c"

#define screenWidth 1000
#define screenHeight 640

typedef enum gameScreen {MENU = 0, NOVOJOGO, CONTINUAR} gameScreen;

int main(void) {
    Rectangle obstaculos[600] ={{0,0,0,0}}; //posX, posY, largura, altura

    // Init
    InitWindow(screenWidth, screenHeight, "Battle INF - MENU");
    gameScreen currentScreen = MENU;

    //Arquivo
    FILE *fileLevel;
    fileLevel = fopen("../levels/nivel1.txt", "r");

    // Load
    Image titulo = LoadImage("../assets/battleInf.png");
    Texture2D tituloTex = LoadTextureFromImage(titulo);
    Font arcade = LoadFont("../assets/ARCADECLASSIC.ttf");

    Image brick = LoadImage("../assets/brick_texture2.png");
    ImageResize(&brick, 25, 40);
    Texture2D brickTexture = LoadTextureFromImage(brick);

    Texture2D personagemUp = LoadTexture("../assets/personagem_up30x30.png");
    Texture2D personagemDown = LoadTexture("../assets/personagem_down30x30.png");
    Texture2D personagemLeft = LoadTexture("../assets/personagem_left30x30.png");
    Texture2D personagemRight = LoadTexture("../assets/personagem_right30x30.png");

    Texture2D energyCellTextura = LoadTexture("../assets/booster25x25.png");

    Image escudo = LoadImage("../assets/shield.png");
    ImageResize(&escudo, 40, 40);
    Texture2D escudoTextura = LoadTextureFromImage(escudo);


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
    personagem.tiro.numBalas = 3;

    //Inicializando PowerUp
    POWERUP powerUp = {0};
    powerUp.posicao.width = 25;
    powerUp.posicao.height = 25;
    powerUp.textura = energyCellTextura;
    do{
        powerUp.posicao.x = GetRandomValue(powerUp.posicao.width, GetScreenWidth()-powerUp.posicao.width);
        powerUp.posicao.y = GetRandomValue(powerUp.posicao.height+40,GetScreenHeight()- powerUp.posicao.height);
    }while(spawnParede(&powerUp, obstaculos, nroBlocos));


    SetTargetFPS(60);
    while (!WindowShouldClose()) { // detecta se vai fechar
        // Update
        switch (currentScreen) {
            case MENU:  {printf("chamando posicao chave\n");
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
                        break;}

            case NOVOJOGO:  {while (readLevel(fileLevel, &positionX, &positionY,&tipo) == 0) { // enquanto tiver coisas para ler
                                if (tipo == '#') { // se a função parou num #, desenha o bloco
                                    brickWall[nroBlocos][0] = positionX;
                                    brickWall[nroBlocos][1] = positionY;

                                    //adicionando as características pro array de structs obstaculos
                                    obstaculos[nroBlocos].x = (positionX-1)*25;
                                    obstaculos[nroBlocos].y = ((positionY-1)*40)+40;
                                    obstaculos[nroBlocos].width = 25.0;
                                    obstaculos[nroBlocos].height = 40.0;
                                    nroBlocos++;
                                }
                                else if (tipo == 'T') { // se parou num T, encontra a posição do jogador
                                    positionPlayer[0][0] = positionX;
                                    positionPlayer[0][1] = positionY;
                                    personagem.posicao.x = (float) (positionX-1)*25;
                                    personagem.posicao.y = (float) (positionY-1)*40+50;

                                }
                            }

                            //Update da posicao e textura do personagem
                            Rectangle posicaoInicial = personagem.posicao;// Guardando posicao inicial antes de colisoes, etc
                            atualizaPosicao(&personagem, personagemRight, personagemLeft, personagemUp, personagemDown);
                            //Colisao Cenario
                            bool ultrapassaCenario = (personagem.posicao.x > screenWidth - personagem.posicao.width) || (personagem.posicao.x <0) || (personagem.posicao.y > screenHeight - personagem.posicao.height) || (personagem.posicao.y <40);
                            if(ultrapassaCenario)
                                personagem.posicao = posicaoInicial;
                            //Colisao Obstaculos
                            checaColisaoArray(&personagem, obstaculos ,nroBlocos,posicaoInicial);
                            break;}

            case CONTINUAR:{ break;}
        }

        // Draw
        BeginDrawing();
            ClearBackground(RAYWHITE);
            switch (currentScreen) {
                case MENU: {//to do
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
                    break;}

                case NOVOJOGO:  {ClearBackground(RAYWHITE);
                                DrawRectangle(0,0,screenWidth,screenHeight,BLACK);
                                for (int j = 0; j < nroBlocos; j++ ) {
                                    int xvec = ((brickWall[j][0]-1)*25);
                                    int yvec = ((brickWall[j][1]-1)*40)+40;
                                    DrawTexture(brickTexture, xvec, yvec, WHITE);
                                 }
                                DrawRectangle(0,0,screenWidth,40,GRAY);

                                administraPowerUp(&powerUp, &personagem, obstaculos, nroBlocos, screenHeight, screenWidth);
                                DrawTexture(personagem.textura, (personagem.posicao.x ), (personagem.posicao.y ), RAYWHITE);
                                administraTiro(&personagem, screenWidth, screenHeight);
                                desenhaCabecalho(&personagem, escudoTextura);
                                break;}

                case CONTINUAR: {DrawRectangle(0, 0, screenWidth, screenHeight, BLACK);
                                DrawTextEx(arcade, "Continuar", (Vector2){200,350}, 75, 1,WHITE);
                                break;}
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

