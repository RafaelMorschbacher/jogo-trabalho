/*
PARA TROCAR DE FASE
- >= 15 tanques destruidos E nenhum tanque na tela
- contador quando inimigo.vida troca
---- também aumenta os pontos
- checar se não tem nenhum tanque na tela
****pode ganhar pontos ao acertar um tijolo

---o sobre pode ser uma parede cheia de tijolos sem inimigos, tem q derrubar tudo pra ler


- salvar com S
- scores
- trocar de nível
- continuar
---se continuar, ainda troca de fase
---ler no documento a fase

*/



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

INIMIGO inimigos[MAX_INIMIGOS] = {};

int maxInimigos =MAX_INIMIGOS;


//variaveis fases
int fimDeJogo;
int inimigosMortos = 0, inimigosEmTela = 0;

#define screenHeight 650

typedef enum gameScreen {MENU = 0, NOVOJOGO, FASE1, FASE2, FASE3, FASE4, SCORES, SOBRE, CONTINUAR} gameScreen;

int main(void) {

    // Init------------------------------------------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "Battle INF - MENU");
    gameScreen currentScreen = MENU;


    //Arquivo
    FILE *Level1p; // lembrar de colocar if else com problema pra abrir arquivo ou nao
    Level1p = fopen("../levels/nivel1.txt", "r");
    FILE *Level2p; // lembrar de colocar if else com problema pra abrir arquivo ou nao
    Level2p = fopen("../levels/nivel2.txt", "r");
    FILE *Level3p; // lembrar de colocar if else com problema pra abrir arquivo ou nao
    Level3p = fopen("../levels/nivel3.txt", "r");
    FILE *Level4p; // lembrar de colocar if else com problema pra abrir arquivo ou nao
    Level4p = fopen("../levels/nivel4.txt", "r");

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

    //Textuta explosão (Inimigo)
    Texture2D explosaoVerde1 = LoadTexture("../assets/inimigo-verde/explosao-verde-1.png");
    Texture2D explosaoVerde2 = LoadTexture("../assets/inimigo-verde/explosao-verde-2.png");
    Texture2D explosaoVerde3 = LoadTexture("../assets/inimigo-verde/explosao-verde-3.png");
    Texture2D explosaoVerde4 = LoadTexture("../assets/inimigo-verde/explosao-verde-4.png");
    Texture2D explosaoVerde5 = LoadTexture("../assets/inimigo-verde/explosao-verde-5.png");
    Texture2D explosaoVerde6 = LoadTexture("../assets/inimigo-verde/explosao-verde-6.png");
    Texture2D explosaoVerde7 = LoadTexture("../assets/inimigo-verde/explosao-verde-7.png");


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
                        currentScreen = FASE1; //onde começa
                if (estadoChave == 2 && IsKeyPressed(KEY_ENTER))
                        currentScreen = CONTINUAR;
                break;
            }

            case FASE1: {
                //lembrar de zerar as variáveis importantes
                fimDeJogo = FALSE;


                criandoMapa (Level1p, &positionX, &positionY, &tipo, obstaculos, &nroBlocos, &personagem);
                criandoInimigos(tempo, &aux, &nroInimigos, &nroBlocos,&corInimigo, inimigos,inimigoRedDown,inimigoGreenDown,obstaculos,&personagem);
                movendoInimigos (screenHeight, &nroInimigos, &nroBlocos, &colisaoInimigoCenario, &colisaoDoInimigo, inimigos, &personagem,  obstaculos, &corInimigo,  inimigoRedUp,  inimigoRedDown,  inimigoRedLeft,  inimigoRedRight,  inimigoGreenUp,  inimigoGreenDown,  inimigoGreenLeft,  inimigoGreenRight);
                movendoPersonagem (&personagem, &nroBlocos, &nroInimigos,  screenHeight, inimigos, obstaculos,  personagemRight,  personagemLeft,  personagemUp,  personagemDown);





                if (inimigosMortos >= 15 && !inimigosEmTela)
                    fimDeJogo == TRUE;


                if (fimDeJogo == TRUE)      //para passagem de fases
                    currentScreen = FASE2;  //falta inserir questão dos scores


                break;
            }

            case FASE2:{
                //lembrar de zerar as variáveis importantes
                fimDeJogo = FALSE;

                criandoMapa (Level2p, &positionX, &positionY, &tipo, obstaculos, &nroBlocos, &personagem);
                criandoInimigos(tempo, &aux, &nroInimigos, &nroBlocos,&corInimigo, inimigos,inimigoRedDown,inimigoGreenDown,obstaculos,&personagem);
                movendoInimigos (screenHeight, &nroInimigos, &nroBlocos, &colisaoInimigoCenario, &colisaoDoInimigo, inimigos, &personagem,  obstaculos, &corInimigo,  inimigoRedUp,  inimigoRedDown,  inimigoRedLeft,  inimigoRedRight,  inimigoGreenUp,  inimigoGreenDown,  inimigoGreenLeft,  inimigoGreenRight);
                movendoPersonagem (&personagem, &nroBlocos, &nroInimigos,  screenHeight, inimigos, obstaculos,  personagemRight,  personagemLeft,  personagemUp,  personagemDown);


                if (inimigosMortos >= 15 && !inimigosEmTela)
                    fimDeJogo == TRUE;

                if (fimDeJogo == TRUE)
                    currentScreen = FASE3;
                break;
            }

            case FASE3:{
                //lembrar de zerar as variáveis importantes
                fimDeJogo = FALSE;

                criandoMapa (Level3p, &positionX, &positionY, &tipo, obstaculos, &nroBlocos, &personagem);
                criandoInimigos(tempo, &aux, &nroInimigos, &nroBlocos,&corInimigo, inimigos,inimigoRedDown,inimigoGreenDown,obstaculos,&personagem);
                movendoInimigos (screenHeight, &nroInimigos, &nroBlocos, &colisaoInimigoCenario, &colisaoDoInimigo, inimigos, &personagem,  obstaculos, &corInimigo,  inimigoRedUp,  inimigoRedDown,  inimigoRedLeft,  inimigoRedRight,  inimigoGreenUp,  inimigoGreenDown,  inimigoGreenLeft,  inimigoGreenRight);
                movendoPersonagem (&personagem, &nroBlocos, &nroInimigos,  screenHeight, inimigos, obstaculos,  personagemRight,  personagemLeft,  personagemUp,  personagemDown);

                if (inimigosMortos >= 15 && !inimigosEmTela)
                    fimDeJogo == TRUE;

                if (fimDeJogo == TRUE)
                    currentScreen = FASE4;


                break;
            }

            case FASE4: {
                fimDeJogo = FALSE;
                //lembrar de zerar as variáveis importantes
                fimDeJogo = FALSE;

                criandoMapa (Level4p, &positionX, &positionY, &tipo, obstaculos, &nroBlocos, &personagem);
                criandoInimigos(tempo, &aux, &nroInimigos, &nroBlocos,&corInimigo, inimigos,inimigoRedDown,inimigoGreenDown,obstaculos,&personagem);
                movendoInimigos (screenHeight, &nroInimigos, &nroBlocos, &colisaoInimigoCenario, &colisaoDoInimigo, inimigos, &personagem,  obstaculos, &corInimigo,  inimigoRedUp,  inimigoRedDown,  inimigoRedLeft,  inimigoRedRight,  inimigoGreenUp,  inimigoGreenDown,  inimigoGreenLeft,  inimigoGreenRight);
                movendoPersonagem (&personagem, &nroBlocos, &nroInimigos,  screenHeight, inimigos, obstaculos,  personagemRight,  personagemLeft,  personagemUp,  personagemDown);


                if (inimigosMortos >= 15 && !inimigosEmTela)
                    fimDeJogo == TRUE;

                if (fimDeJogo == TRUE)
                    currentScreen = SOBRE;
                break;

            }

            case CONTINUAR: {
                break;
            }

            case SOBRE: {
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

                case FASE1: {
                    ClearBackground(RAYWHITE);
                    DrawRectangle(0,0,screenWidth,screenHeight,BLACK);

                    for (int j = 0; j < nroBlocos; j++ ) {
                        int xvec = obstaculos[j].posicao.x;
                        int yvec =  obstaculos[j].posicao.y;
                        if (!obstaculos[j].destruido)
                            DrawTexture(brickTexture, xvec, yvec, WHITE);
                    }
                    //DrawRectangleRec

                    administraPowerUp(&powerUp, &personagem, obstaculos, nroBlocos, screenHeight, screenWidth);
                    administraTiro(&personagem, screenWidth, screenHeight, obstaculos , nroBlocos,  inimigos, nroInimigos, &maxInimigos, &inimigosMortos, &inimigosEmTela);
                    DrawTexture(personagem.textura, (personagem.posicao.x ), (personagem.posicao.y ), RAYWHITE);
                    desenhaCabecalho(&personagem, escudoTextura, arcade, fase[0]);

                    //desenhando inimigos na tela
                    for (int i = 0; i<nroInimigos; i++) {

                        if(inimigos[i].vivo==FALSE){
                            DrawTexture(inimigoDead, (inimigos[i].posicao.x),  (inimigos[i].posicao.y), RAYWHITE);
                            int numFrames = 5;

                                if(inimigos[i].contadorAnimacao <= numFrames)
                                    DrawTexture(inimigos[i].textura, (inimigos[i].posicao.x),  (inimigos[i].posicao.y), RAYWHITE);
                                else if(inimigos[i].contadorAnimacao <= numFrames*2)
                                    DrawTexture(explosaoVerde2, (inimigos[i].posicao.x),  (inimigos[i].posicao.y), RAYWHITE);
                                else if(inimigos[i].contadorAnimacao <= numFrames*3)
                                    DrawTexture(explosaoVerde3, (inimigos[i].posicao.x),  (inimigos[i].posicao.y), RAYWHITE);
                                else if(inimigos[i].contadorAnimacao <= numFrames*4)
                                    DrawTexture(explosaoVerde4, (inimigos[i].posicao.x),  (inimigos[i].posicao.y), RAYWHITE);
                                else if(inimigos[i].contadorAnimacao <= numFrames*5)
                                    DrawTexture(explosaoVerde5, (inimigos[i].posicao.x),  (inimigos[i].posicao.y), RAYWHITE);
                                else if(inimigos[i].contadorAnimacao <= numFrames*6)
                                    DrawTexture(explosaoVerde6, (inimigos[i].posicao.x),  (inimigos[i].posicao.y), RAYWHITE);
                                else if(inimigos[i].contadorAnimacao <= numFrames*7)
                                    DrawTexture(explosaoVerde7, (inimigos[i].posicao.x),  (inimigos[i].posicao.y), RAYWHITE);
                                else
                                    DrawTexture(inimigoDead, (inimigos[i].posicao.x),  (inimigos[i].posicao.y), RAYWHITE);

                                inimigos[i].contadorAnimacao += 1;
                        } //testando

                        else {
                            administraTiroInimigos( &inimigos[i],  screenWidth, screenHeight, obstaculos,  nroBlocos,  &personagem);
                            DrawTexture(inimigos[i].textura, (inimigos[i].posicao.x),  (inimigos[i].posicao.y), RAYWHITE);
                        }


                    }
                    break;

                }

                 case FASE2:{
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
                    administraTiro(&personagem, screenWidth, screenHeight, obstaculos, nroBlocos, inimigos, nroInimigos, &maxInimigos, &inimigosMortos, &inimigosEmTela);
                    DrawTexture(personagem.textura, (personagem.posicao.x ), (personagem.posicao.y ), RAYWHITE);
                    desenhaCabecalho(&personagem, escudoTextura, arcade, fase[0]);

                    //desenhando inimigos na tela
                    for (int i = 0; i<nroInimigos; i++) {
                        if(inimigos[i].vivo==TRUE)
                            DrawTexture(inimigos[i].textura, (inimigos[i].posicao.x),  (inimigos[i].posicao.y), RAYWHITE);
                        if(inimigos[i].vivo==FALSE){
                                DrawTexture(inimigoDead, (inimigos[i].posicao.x),  (inimigos[i].posicao.y), RAYWHITE);
                        }


                        //Tiro dos inimigos
                        administraTiroInimigos( &inimigos[i], screenWidth, screenHeight, obstaculos, nroBlocos, &personagem);

                    }
                    break;
                }

                case FASE3:{
                    ClearBackground(RAYWHITE);
                    DrawRectangle(0,0,screenWidth,screenHeight,BLACK);

                    for (int j = 0; j < nroBlocos; j++ ) {
                        int xvec = obstaculos[j].posicao.x;
                        int yvec =  obstaculos[j].posicao.y;
                        DrawTexture(brickTexture, xvec, yvec, WHITE);
                    }
                    //DrawRectangleRec

                    administraPowerUp(&powerUp, &personagem, obstaculos, nroBlocos, screenHeight, screenWidth);
                     administraTiro(&personagem, screenWidth, screenHeight, obstaculos, nroBlocos, inimigos, nroInimigos, &maxInimigos, &inimigosMortos, &inimigosEmTela);
                    DrawTexture(personagem.textura, (personagem.posicao.x ), (personagem.posicao.y ), RAYWHITE);
                    desenhaCabecalho(&personagem, escudoTextura, arcade, fase[0]);

                    //desenhando inimigos na tela
                    for (int i = 0; i<nroInimigos; i++) {
                        DrawTexture(inimigos[i].textura, (inimigos[i].posicao.x),  (inimigos[i].posicao.y), RAYWHITE);
                    }
                    break;
                }

                case FASE4: {
                    ClearBackground(RAYWHITE);
                    DrawRectangle(0,0,screenWidth,screenHeight,BLACK);

                    for (int j = 0; j < nroBlocos; j++ ) {
                        int xvec = obstaculos[j].posicao.x;
                        int yvec =  obstaculos[j].posicao.y;
                        DrawTexture(brickTexture, xvec, yvec, WHITE);
                    }
                    //DrawRectangleRec

                    administraPowerUp(&powerUp, &personagem, obstaculos, nroBlocos, screenHeight, screenWidth);
                     administraTiro(&personagem, screenWidth, screenHeight, obstaculos, nroBlocos, inimigos, nroInimigos, &maxInimigos, &inimigosMortos, &inimigosEmTela);
                    DrawTexture(personagem.textura, (personagem.posicao.x ), (personagem.posicao.y ), RAYWHITE);
                    desenhaCabecalho(&personagem, escudoTextura, arcade, fase[0]);

                    //desenhando inimigos na tela
                    for (int i = 0; i<nroInimigos; i++) {
                        DrawTexture(inimigos[i].textura, (inimigos[i].posicao.x),  (inimigos[i].posicao.y), RAYWHITE);
                    }
                    break;
                }

                case CONTINUAR:{
                    DrawRectangle(0, 0, screenWidth, screenHeight, BLACK);
                    DrawTextEx(arcade, "Continuar", (Vector2){200,350}, 75, 1,WHITE);
                    break;
                }

                case SOBRE: {
                    break;
                }

                default: break;
            }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    //Unload
    UnloadTexture(tituloTex);
    fclose (Level1p);
    fclose (Level2p);
    fclose (Level3p);
    fclose (Level4p);

    CloseWindow();
    return 0;
}

