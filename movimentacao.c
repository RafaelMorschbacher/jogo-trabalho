#include "raylib.h"
#define VELOCIDADE_INICIAL 4
#define NUM_OBSTACULOS 5

        //Estrutura Personagem
typedef struct personagem{
    Rectangle posicao;
    Texture2D textura;
    int velocidadeBase;
    int velocidadeAtual;
    int vidas;
    }PERSONAGEM;

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

void administraBooster(Rectangle *booster, Texture2D boosterTextura, PERSONAGEM *personagem, int larguraTela, int alturaTela)
{
    if(CheckCollisionRecs((*personagem).posicao, *booster))
    {
        (*personagem).velocidadeAtual += (*personagem).velocidadeBase;
        (*booster).x = GetRandomValue((*booster).width, larguraTela-(*booster).width);
        (*booster).y = GetRandomValue((*booster).height,alturaTela-(*booster).height);
        }
        else
        {
            DrawRectangleRec(*booster, BLANK);
            DrawTexture(boosterTextura, (*booster).x, (*booster).y, RAYWHITE);
    }
}

void atualizaPosicao(PERSONAGEM *personagem , Texture personagemRight, Texture personagemLeft, Texture personagemUp, Texture personagemDown)
{

    if(IsKeyDown(KEY_RIGHT))
        {
            (*personagem).posicao.x += (*personagem).velocidadeAtual;
             (*personagem).textura = personagemRight;
        }


        if(IsKeyDown(KEY_LEFT))
        {
             (*personagem).posicao.x -=  (*personagem).velocidadeAtual;
             (*personagem).textura = personagemLeft;
        }

        if(IsKeyDown(KEY_UP))
        {
             (*personagem).posicao.y -=  (*personagem).velocidadeAtual;
             (*personagem).textura = personagemUp;
        }


        if(IsKeyDown(KEY_DOWN))
        {
             (*personagem).posicao.y +=  (*personagem).velocidadeAtual;
             (*personagem).textura = personagemDown;
        }

}
void desenhaCabecalho(PERSONAGEM *personagem, Texture2D iconeVidas)
{
        DrawText("Vidas: ", 100, 15, 20, LIGHTGRAY);

        int espacamento = 0;
        if((*personagem).vidas>0)
        {
            for(int i= (*personagem).vidas; i>0; i--)
            {
                DrawTexture(iconeVidas, (180 + espacamento), 10 , RAYWHITE);
                espacamento +=40;
            }
        }

}
int main()
{

//////////Iniciando////////////


    const int larguraTela = 1000;
    const int alturaTela = 600;

    InitWindow(larguraTela, alturaTela, "primeiraJanela");

    SetTargetFPS(60);


    //Rectangle personagem = { larguraTela/2, alturaTela/2, 25, 25 };
    Rectangle obstaculos[NUM_OBSTACULOS] ={{400, 300, 100, 100},{100, 100, 50, 100}, {600,100,40,40} };

    Rectangle booster = {GetRandomValue(25, larguraTela-25),GetRandomValue(25, alturaTela-25) , 25, 25};

////////Imagens e Texturas//////////

    Texture2D personagemUp = LoadTexture("assets/personagem_up30x30.png");
    Texture2D personagemDown = LoadTexture("assets/personagem_down30x30.png");
    Texture2D personagemLeft = LoadTexture("assets/personagem_left30x30.png");
    Texture2D personagemRight = LoadTexture("assets/personagem_right30x30.png");

    //Textura do Booster

    Texture2D boosterTextura = LoadTexture("assets/booster25x25.png");

/////////Inicialização do Personagem/////////////

    PERSONAGEM personagem = {0};

    personagem.posicao.x = larguraTela/2;
    personagem.posicao.y = alturaTela/2;
    personagem.posicao.width = 25;
    personagem.posicao.height = 25;

    personagem.velocidadeBase = VELOCIDADE_INICIAL;
    personagem.velocidadeAtual = personagem.velocidadeBase;
    personagem.textura = personagemUp;
    personagem.vidas = 3;

/////////Loop do Jogo////////////


    while(!WindowShouldClose())
    {

    ///////Update da posicao e textura do personagem//////////

        Rectangle posicaoInicial = personagem.posicao;// Guardando posicao inicial antes de colisoes, etc

        atualizaPosicao(&personagem, personagemRight, personagemLeft, personagemUp, personagemDown);

        DrawTexture(personagem.textura, (personagem.posicao.x ), (personagem.posicao.y ), RAYWHITE);


        /////Colisao Cenario///////

        bool ultrapassaCenario = (personagem.posicao.x > larguraTela - personagem.posicao.width) || (personagem.posicao.x <0) || (personagem.posicao.y > alturaTela - personagem.posicao.height) || (personagem.posicao.y <0);
        if(ultrapassaCenario) personagem.posicao = posicaoInicial;

        /////Colisao Obstaculos/////////////

        checaColisaoArray(&personagem.posicao, obstaculos,NUM_OBSTACULOS,posicaoInicial);


    ///////Desenho/////////

        BeginDrawing();

        ////Cabeçalho (vidas)///////

        desenhaCabecalho(&personagem, personagemUp);

        ///////Cenario///////////

        ClearBackground(RAYWHITE);

        ///////Obstáculos////////////

            //Desenhando cada bloco da lista de obstaculos
           for(int i=0; i<NUM_OBSTACULOS-1; i++)
           {
               DrawRectangleRec(obstaculos[i], RED);
           }


           //BOOSTER (poderzinho)

           administraBooster(&booster, boosterTextura, &personagem, alturaTela, larguraTela);



        EndDrawing();

    }

////////////////Finaliza��o///////////

    CloseWindow();

    return 0;



}
