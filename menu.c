// esse é um teste para aprender a usar o GitHub!
/*
Comandos Básicos
--- DrawRectangle(int posX, int posY, int width, int height, Color color);

*/

#include <stdio.h>
#include <raylib.h> 


int main(void) {

    const int screenWidth = 1000;
    const int screenHeight = 600;
   // Image muro = LoadImage("../sourceImage/muro.png");
    //ImageDrawText(&muro, "BATTLE INF", 200, 50, 90, GRAY); 
    SetTargetFPS(60); 

    InitWindow(screenWidth, screenHeight, "Battle INF - MENU");

    while (!WindowShouldClose()) { // detecta se vai fechar

        // Draw
        //--------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            DrawRectangle(0, 0, screenWidth, screenHeight, BLACK);
            DrawText("BATTLE INF", 200, 50, 90, GRAY);
           
            DrawText("Novo jogo\nContinuar\nSair",380, 250, 40, GRAY);
             

        EndDrawing();
        //----------------------------------------------------------------------------------


    }
    CloseWindow(); 
    return 0;  
}