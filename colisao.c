#include "raylib.h"


int main()
{

//////////Iniciando////////////


    const int larguraTela = 800;
    const int alturaTela = 450;

    InitWindow(larguraTela, alturaTela, "primeiraJanela");

    SetTargetFPS(60);

    Rectangle personagem = { larguraTela/2, alturaTela/2, 50, 50 };
    Rectangle obstaculo = {400, 300, 100, 100};
    Rectangle obstaculo2 = {100, 100, 50, 100};

/////////Variáveis auxiliares////////////

    int velocidade = 8;


/////////Loop do Jogo////////////


    while(!WindowShouldClose())
    {

    ///////Update//////////

        if(IsKeyDown(KEY_RIGHT)) personagem.x += velocidade;
        if(IsKeyDown(KEY_LEFT)) personagem.x -= velocidade;
        if(IsKeyDown(KEY_UP)) personagem.y -= velocidade;
        if(IsKeyDown(KEY_DOWN)) personagem.y += velocidade;


    ///////Desenho/////////

        BeginDrawing();

        ///////Cenario///////////

            ClearBackground(RAYWHITE);
            DrawText("Mexa com as setas", 10, 10, 30, LIGHTGRAY);

        ///////Blocos////////////

            DrawRectangleRec(personagem, GREEN);
            DrawRectangleRec(obstaculo, RED);
            DrawRectangleRec(obstaculo2, RED);

        /////Colisao/////////////

        bool colisao = CheckCollisionRecs(personagem,obstaculo) || CheckCollisionRecs(personagem,obstaculo2);

        if(colisao)
            {
                if(IsKeyDown(KEY_RIGHT)) personagem.x -= velocidade;
                if(IsKeyDown(KEY_LEFT)) personagem.x += velocidade;
                if(IsKeyDown(KEY_UP)) personagem.y += velocidade;
                if(IsKeyDown(KEY_DOWN)) personagem.y -= velocidade;
            }

        EndDrawing();

    }

////////////////Finalização///////////

    CloseWindow();

    return 0;



}
