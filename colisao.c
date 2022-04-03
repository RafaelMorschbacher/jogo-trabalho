#include "raylib.h"


int main()
{

//////////Iniciando////////////


    const int larguraTela = 1000;
    const int alturaTela = 600;

    InitWindow(larguraTela, alturaTela, "primeiraJanela");

    SetTargetFPS(60);

    Rectangle personagem = { larguraTela/2, alturaTela/2, 25, 25 };
    Rectangle obstaculo = {400, 300, 100, 100};
    Rectangle obstaculo2 = {100, 100, 50, 100};

/////////Variáveis auxiliares////////////

    int velocidade = 8;

////////Imagens//////////

    Texture2D personagemUp = LoadTexture("./assets/personagem_up30x30.png");
    Texture2D personagemDown = LoadTexture("./assets/personagem_down30x30.png");
    Texture2D personagemLeft = LoadTexture("./assets/personagem_left30x30.png");
    Texture2D personagemRight = LoadTexture("./assets/personagem_right30x30.png");
    Texture2D texturaAtual = personagemUp;


/////////Loop do Jogo////////////


    while(!WindowShouldClose())
    {

    ///////Update//////////

        Rectangle posicaoInicial = personagem;

        //Atualização da posição e textura do jogador

        if(IsKeyDown(KEY_RIGHT))
        {
            personagem.x += velocidade;
            texturaAtual = personagemRight;
        }


        if(IsKeyDown(KEY_LEFT))
        {
            personagem.x -= velocidade;
            texturaAtual = personagemLeft;
        }

        if(IsKeyDown(KEY_UP))
        {
            personagem.y -= velocidade;
            texturaAtual = personagemUp;
        }


        if(IsKeyDown(KEY_DOWN))
        {
            personagem.y += velocidade;
            texturaAtual = personagemDown;
        }

        DrawTexture(texturaAtual, (personagem.x ), (personagem.y ), RAYWHITE);


        /////Colisao Cenario///////

        bool ultrapassaCenario = (personagem.x > larguraTela - personagem.width) || (personagem.x <0) || (personagem.y > alturaTela - personagem.height) || (personagem.y <0);
        if(ultrapassaCenario) personagem = posicaoInicial;

        /////Colisao Obstaculos/////////////

       bool colide = CheckCollisionRecs(personagem,obstaculo) || CheckCollisionRecs(personagem,obstaculo2);

       if(colide) personagem = posicaoInicial;


    ///////Desenho/////////

        BeginDrawing();

        ///////Cenario///////////

            ClearBackground(RAYWHITE);
            DrawText("Mexa com as setas", 10, 10, 30, LIGHTGRAY);




        ///////Blocos////////////

            DrawRectangleRec(personagem, BLANK);
            DrawRectangleRec(obstaculo, RED);
            DrawRectangleRec(obstaculo2, RED);




        EndDrawing();

    }

////////////////Finalização///////////

    CloseWindow();

    return 0;



}
