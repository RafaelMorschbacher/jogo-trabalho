#include <stdio.h>
#include <raylib.h>
#include <string.h>
#include "bibjogo.c"


#define screenWidth 1000
#define screenHeight 600
typedef enum gameScreen {MENU = 0, NOVOJOGO, CONTINUAR} gameScreen;


int main(void) {
    printf ("aqui dentro do main\n"); 
    // variaveis
    char menuOptions[3][10] =  {"Novo Jogo", "Continuar","Sair"};

    float u;

    int positionX = 0;                  //coordenada x de um objeto na tela
    int positionY = 1;                  //coordenada y de um objeto na tela
    int brickWall[600][2] = {0};
    int positionPlayer[1][2] = {0};
    int nroBlocos = 0;
    char tipo = ' ';

    int estadoChave = 1;


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

    // Preechendo vetor de posiçoes de blocos
    // Depois provavelmente vai pro UPDATE, aí, atualizar o vetor a cada vez, pra ver se alguma peça foi tirada (com um tiro), e ele 
    //...vai printar novamente na tela, sem aquele que saiu dentro do vetor
    printf("entrando no while q chama a read level"); 
    while (readLevel(fileLevel, &positionX, &positionY,&tipo) == 0) { // enquanto tiver coisas para ler
        if (tipo == '#') { // se a função parou num #, desenha o bloco
            brickWall[nroBlocos][0] = positionX;
            brickWall[nroBlocos][1] = positionY;
            nroBlocos++;
        }
        else if (tipo == 'T') { // se parou num T, encontra a posição do jogador
            positionPlayer[0][0] = positionX;
            positionPlayer[0][1] = positionY;
        }
    }

    SetTargetFPS(60);
    while (!WindowShouldClose()) { // detecta se vai fechar
        // Update
        switch (currentScreen) {
            case MENU:  printf("chamando posicao chave\n");
                 
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
                        
            case NOVOJOGO: break;;
            case CONTINUAR: break;;
        }
        // Draw
        BeginDrawing();
            ClearBackground(RAYWHITE);
            switch (currentScreen) {
                case MENU: //to do
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

                case NOVOJOGO:  InitWindow(screenWidth, 640, "Novo Jogo");
                                ClearBackground(RAYWHITE);
                                DrawRectangle(0,0,screenWidth,screenHeight,BLACK);
                                DrawRectangle(0,0,screenWidth,40,GRAY);

                                for (int j = 0; j < nroBlocos; j++ ) {
                                    int xvec = ((brickWall[j][0]-1)*25);
                                    int yvec = ((brickWall[j][1]-1)*40)+40;
                                    DrawTexture(brickTexture, xvec, yvec, WHITE);
                                break; }

                case CONTINUAR: DrawRectangle(0, 0, screenWidth, screenHeight, BLACK);
                                DrawTextEx(arcade, "Continuar", (Vector2){200,350}, 75, 1,WHITE);
                                break;
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

