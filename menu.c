#include <stdio.h>
#include <raylib.h>
#include <string.h>

#define screenWidth 1000
#define screenHeight 600
typedef enum gameScreen {MENU = 0, NOVOJOGO, CONTINUAR} gameScreen;

float alignCenterFont (char *v, int i, int fontSize, Font arcade) { //função para alinhar
    float width;
    Vector2 tamText = MeasureTextEx(arcade, v, fontSize, 1);
    width = ((screenWidth-tamText.x)/2);
    return width;
}

int main(void) {

    // variaveis
    char menuOptions[3][10] =  {"Novo Jogo", "Continuar","Sair"};
    int estadoChave = 1;
    float u;

    // Init
    InitWindow(screenWidth, screenHeight, "Battle INF - MENU");
    gameScreen currentScreen = MENU;

    // Load
    Image titulo = LoadImage("../assets/battleInf.png");
    Texture2D tituloTex = LoadTextureFromImage(titulo);
    Font arcade = LoadFont("../assets/ARCADECLASSIC.ttf");



    SetTargetFPS(60);
    while (!WindowShouldClose()) { // detecta se vai fechar

        // Update
        switch (currentScreen) {
            case MENU:
                //da posição da chave, pra saber onde deve estar amarelo
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

                    //escreve menu
                    u = 0;
                    for (int i = 0; i < 3; i++) {
                        DrawTextEx(arcade, menuOptions[i], (Vector2){(alignCenterFont(menuOptions[i],i,40, arcade)),(250 + u) }, 40, 1,GRAY);
                        u+= 50.0;
                    }
                    // deixa amarelo
                    switch (estadoChave) {
                        case 1:
                            DrawTextEx(arcade, menuOptions[0], (Vector2){(alignCenterFont(menuOptions[0],0,40, arcade)),250 }, 40, 1,YELLOW);
                            break;
                        case 2:
                            DrawTextEx(arcade, menuOptions[1], (Vector2){(alignCenterFont(menuOptions[1],1,40, arcade)),(300) }, 40, 1,YELLOW);
                            break;
                        case 3:
                            DrawTextEx(arcade, menuOptions[2], (Vector2){(alignCenterFont(menuOptions[2],2,40, arcade)),(350) }, 40, 1,YELLOW);
                            break;
                        default: break;
                    }
                    break;

                case NOVOJOGO:
                    DrawRectangle(0, 0, screenWidth, screenHeight, BLACK);
                    DrawTextEx(arcade, "Novo Jogo", (Vector2){200,350}, 75, 1,WHITE);
                    break;

                case CONTINUAR:
                    DrawRectangle(0, 0, screenWidth, screenHeight, BLACK);
                    DrawTextEx(arcade, "Continuar", (Vector2){200,350}, 75, 1,WHITE);
                    break;
                default: break;
            }


        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    //Unload
    UnloadTexture(tituloTex);


    CloseWindow();
    return 0;
}
