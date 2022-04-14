#include "raylib.h"
#define VELOCIDADE_INICIAL 4
#define NUM_OBSTACULOS 5

void checaColisao(Rectangle *personagem, Rectangle *obstaculo, Rectangle posicaoInicial)
{
    if(CheckCollisionRecs(*personagem,*obstaculo))
        *personagem = posicaoInicial;
}

void checaColisaoArray(Rectangle *personagem, Rectangle *obstaculos, int numObstaculos, Rectangle posicaoInicial)
{
        for(int i=0; i<numObstaculos-1; i++)
        {
            checaColisao(personagem, &obstaculos[i], posicaoInicial);
        }

}

void administraBooster(Rectangle *booster, Texture2D boosterTextura, Rectangle *personagem, int *velocidade, int larguraTela, int alturaTela)
{
    if(CheckCollisionRecs(*personagem, *booster))
    {
        *velocidade += *velocidade/2;
        (*booster).x = GetRandomValue((*booster).width, larguraTela-(*booster).width);
        (*booster).y = GetRandomValue((*booster).height,alturaTela-(*booster).height);
        }
        else
        {
            DrawRectangleRec(*booster, BLANK);
            DrawTexture(boosterTextura, (*booster).x, (*booster).y, RAYWHITE);
    }
}

int main()
{

//////////Iniciando////////////


    const int larguraTela = 1000;
    const int alturaTela = 600;

    InitWindow(larguraTela, alturaTela, "primeiraJanela");

    SetTargetFPS(60);

    Rectangle personagem = { larguraTela/2, alturaTela/2, 25, 25 };
    Rectangle obstaculos[NUM_OBSTACULOS] ={{400, 300, 100, 100},{100, 100, 50, 100}, {600,100,40,40} };

    Rectangle booster = {GetRandomValue(25, larguraTela-25),GetRandomValue(25, alturaTela-25) , 25, 25};

/////////Vari�veis auxiliares////////////

    int velocidade = VELOCIDADE_INICIAL;

////////Imagens e Texturas//////////

    Texture2D personagemUp = LoadTexture("assets/personagem_up30x30.png");
    Texture2D personagemDown = LoadTexture("assets/personagem_down30x30.png");
    Texture2D personagemLeft = LoadTexture("assets/personagem_left30x30.png");
    Texture2D personagemRight = LoadTexture("assets/personagem_right30x30.png");
    //Textura inicial do personagem � virado para cima
    Texture2D texturaAtual = personagemUp;

    //Textura do Booster

    Texture2D boosterTextura = LoadTexture("assets/booster25x25.png");


/////////Loop do Jogo////////////


    while(!WindowShouldClose())
    {

    ///////Update//////////

        Rectangle posicaoInicial = personagem;

        //Atualiza��o da posi��o e textura do jogador

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

        checaColisaoArray(&personagem, obstaculos,NUM_OBSTACULOS,posicaoInicial);


    ///////Desenho/////////

        BeginDrawing();

        ///////Cenario///////////

            ClearBackground(RAYWHITE);
            DrawText("Mexa com as setas", 10, 10, 30, LIGHTGRAY);




        ///////Blocos////////////

            DrawRectangleRec(personagem, BLANK);

            //Desenhando cada bloco da lista de obstaculos
           for(int i=0; i<NUM_OBSTACULOS-1; i++)
           {
               DrawRectangleRec(obstaculos[i], RED);
           }


           //BOOSTER (poderzinho)

           administraBooster(&booster, boosterTextura, &personagem, &velocidade, alturaTela, larguraTela);



        EndDrawing();

    }

////////////////Finaliza��o///////////

    CloseWindow();

    return 0;



}
