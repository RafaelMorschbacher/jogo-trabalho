#include "raylib.h"
#define VELOCIDADE_INICIAL 4
#define NUM_OBSTACULOS 5

typedef struct personagem{
    Rectangle posicao;
    int inclinacao;
    Texture2D textura;
    int velocidadeBase;
    int velocidadeAtual;
    int vidas;
    }PERSONAGEM;
typedef struct powerUp{
    Rectangle posicao;
    int ativo;
    int cooldown;
    Texture2D textura;
    }POWERUP;
void checaColisao(Rectangle *personagem, Rectangle *obstaculo, Rectangle posicaoInicial)
{
    if(CheckCollisionRecs(*personagem,*obstaculo))
        *personagem = posicaoInicial;
}

void checaColisaoArray(Rectangle *personagem, Rectangle *obstaculos, int numObstaculos, Rectangle posicaoInicial)
{
        for(int i=0; i<numObstaculos-1; i++)
        {
            checaColisao(&(*personagem), &obstaculos[i], posicaoInicial);
        }

}

void administraPowerUp(POWERUP *powerUp, PERSONAGEM *personagem, int larguraTela, int alturaTela)
{
    if(CheckCollisionRecs(personagem->posicao, powerUp->posicao))
    {
        //Power-up coletado
        personagem->velocidadeAtual = personagem->velocidadeBase * 1.5;
        powerUp->cooldown = 5*60;
        powerUp->ativo = 1;
        powerUp->posicao.x = GetRandomValue((*powerUp).posicao.width, larguraTela-(*powerUp).posicao.width);
        powerUp->posicao.y = GetRandomValue((*powerUp).posicao.height,alturaTela-(*powerUp).posicao.height);
        }
        else
        {
            if(!powerUp->ativo)
            {
                DrawRectangleRec(powerUp->posicao, BLANK);
                DrawTexture(powerUp->textura, powerUp->posicao.x, powerUp->posicao.y, RAYWHITE);
            }
            else
            {
                powerUp->cooldown--;
            }

            if(powerUp->cooldown <=0)
            {
                powerUp->ativo = 0;
                personagem->velocidadeAtual = personagem->velocidadeBase;
            }



    }
}

void atualizaPosicao(PERSONAGEM *personagem , Texture personagemRight, Texture personagemLeft, Texture personagemUp, Texture personagemDown)
{

    if(IsKeyDown(KEY_RIGHT))
        {
            personagem->posicao.x += (*personagem).velocidadeAtual;
            personagem->textura = personagemRight;
            personagem->inclinacao = 0;
        }


        if(IsKeyDown(KEY_LEFT))
        {
             personagem->posicao.x -=  (*personagem).velocidadeAtual;
             personagem->textura = personagemLeft;
             personagem->inclinacao = 180;
        }

        if(IsKeyDown(KEY_UP))
        {
             personagem->posicao.y -=  (*personagem).velocidadeAtual;
             personagem->textura = personagemUp;
             personagem->inclinacao = 0;
        }


        if(IsKeyDown(KEY_DOWN))
        {
             personagem->posicao.y +=  (*personagem).velocidadeAtual;
             personagem->textura = personagemDown;
             personagem->inclinacao = 270;
        }

}
void desenhaCabecalho(PERSONAGEM *personagem, Texture2D iconeVidas)
{
        DrawText("Vidas: ", 100, 15, 20, LIGHTGRAY);

        int espacamento = 0;
        if(personagem->vidas>0)
        {
            for(int i= personagem->vidas; i>0; i--)
            {
                DrawTexture(iconeVidas, (180 + espacamento), 10 , RAYWHITE);
                espacamento +=40;
            }
        }

         DrawText("FASE 1", 440, 15, 30, LIGHTGRAY);
}
int main()
{

//////////Iniciando////////////


    const int larguraTela = 1000;
    const int alturaTela = 600;

    InitWindow(larguraTela, alturaTela, "primeiraJanela");

    SetTargetFPS(60);

    Rectangle obstaculos[NUM_OBSTACULOS] ={{400, 300, 100, 100},{100, 100, 50, 100}, {600,100,40,40} };


////////Imagens e Texturas//////////

    Texture2D personagemUp = LoadTexture("assets/personagem_up30x30.png");
    Texture2D personagemDown = LoadTexture("assets/personagem_down30x30.png");
    Texture2D personagemLeft = LoadTexture("assets/personagem_left30x30.png");
    Texture2D personagemRight = LoadTexture("assets/personagem_right30x30.png");

    Texture2D energyCellTextura = LoadTexture("assets/booster25x25.png");

    Image escudo = LoadImage("assets/shield.png");
    ImageResize(&escudo, 40, 40);
    Texture2D escudoTextura = LoadTextureFromImage(escudo);




/////////Inicialização do Personagem/////////////

    PERSONAGEM personagem = {0};

    personagem.posicao.x = larguraTela/2;
    personagem.posicao.y = alturaTela/2;
    personagem.posicao.width = 25;
    personagem.posicao.height = 25;

    personagem.inclinacao = 90;
    personagem.velocidadeBase = VELOCIDADE_INICIAL;
    personagem.velocidadeAtual = personagem.velocidadeBase;
    personagem.textura = personagemUp;
    personagem.vidas = 3;

    //Inicializando PowerUp

    POWERUP powerUp = {0};
    powerUp.posicao.x = GetRandomValue(25, larguraTela-25);
    powerUp.posicao.y = GetRandomValue(25, alturaTela-25);
    powerUp.posicao.width = 25;
    powerUp.posicao.height = 25;
    powerUp.textura = energyCellTextura;

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

        desenhaCabecalho(&personagem, escudoTextura);

        ///////Cenario///////////

        ClearBackground(RAYWHITE);

        ///////Obstáculos////////////

            //Desenhando cada bloco da lista de obstaculos
           for(int i=0; i<NUM_OBSTACULOS-1; i++)
           {
               DrawRectangleRec(obstaculos[i], RED);
           }


           //BOOSTER (poderzinho)

           administraPowerUp(&powerUp, &personagem, alturaTela, larguraTela);



        EndDrawing();

    }

////////////////Finaliza��o///////////

    CloseWindow();

    return 0;



}
