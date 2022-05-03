
#include "bibjogo.h"
//#include "bibjogo.c"


#define screenHeight 650

typedef enum gameScreen {MENU = 0, FASE1, FASE2, FASE3, FASE4, SCORES, SOBRE, CONTINUAR, ESCOLHAFASE, NOME} gameScreen;

int main(void) {


    //VARIÁVEIS ------------------------------------------------------------------------------------------------------------------------
    //variaveis menu
    char menuOptions[6][15] =  {"Novo Jogo", "Continuar","Fases", "Sobre", "High Scores", "Sair"};
    float u;
    char tipo = ' ';
    int estadoChave = 1;
    char fase[4][10] = {"FASE 1","FASE 2", "FASE 3", "FASE 4"};

    //variaveis obstaculos
    float positionX = 0;                  //coordenada x de um objeto na tela
    float positionY = 1;                  //coordenada y de um objeto na tela
    int nroBlocos = 0;
    OBSTACULO obstaculos[600] ={0}; //posX, posY, largura, altura
   
    //variaveis inimigos
    int nroInimigos = 0;
    clock_t tempo[2];
    int aux = 1, aux1 = 1, aux2 = 1, aux3 = 1, auxC = 1, auxM = 1, auxSobre = 1;
    char corInimigo;
    int colisaoDoInimigo = FALSE;
    int colisaoInimigoCenario = FALSE;
    INIMIGO inimigos[MAX_INIMIGOS] = {};
    int maxInimigos = MAX_INIMIGOS;


    //variaveis fases
    int fimDeJogo = FALSE;
    int inimigosMortos = 0, inimigosEmTela = 0;
    
    //variaveis escolher fase
    int estadoChaveFase = 1; 
    int faseUnica = FALSE; 

    //variaveis highscore
    HIGHSCORE recordistas[5] = {}; 
    int recorde = 0; 

    //variaveis inserir nome
    int letterCount = 0;
    Rectangle textBox = { screenWidth/2.0f - 210, 300, 420, 50 };
    bool mouseOnText = false;
    int framesCounter = 0;


    // Init------------------------------------------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "Battle INF");
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
    FILE *sobreP; // lembrar de colocar if else com problema pra abrir arquivo ou nao
    sobreP = fopen("../levels/sobre.txt", "r");

    FILE *savePointer;
    savePointer = fopen("../levels/continuar.txt", "rb");

    

    


    // Load------------------------------------------------------------------------------------------------------------------------
    Image titulo = LoadImage("../assets/battleInf.png");
    Texture2D tituloTex = LoadTextureFromImage(titulo);
    Font arcade = LoadFont("../assets/ARCADECLASSIC.ttf");

    //texturas obstaculos

    Image brick = LoadImage("../assets/brick_texture2.png");
    ImageResize(&brick, 25, 40);
    Texture2D brickTexture = LoadTextureFromImage(brick);


    //explosao obstaculos
   // Texture2D explosion = LoadTexture("../assets/explosion.png");
    



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

    //Textuta explosão (Obstaculo)
    Texture2D expObst1 = LoadTexture("../assets/tijolo/explosao-tijolo-1.png");
    Texture2D expObst2 = LoadTexture("../assets/tijolo/explosao-tijolo-2.png");
    Texture2D expObst3 = LoadTexture("../assets/tijolo/explosao-tijolo-3.png");
    Texture2D expObst4 = LoadTexture("../assets/tijolo/explosao-tijolo-4.png");
    Texture2D expObst5 = LoadTexture("../assets/tijolo/explosao-tijolo-5.png");
    Texture2D expObst6 = LoadTexture("../assets/tijolo/explosao-tijolo-6.png");
    Texture2D expObst7 = LoadTexture("../assets/tijolo/explosao-tijolo-7.png");

    //Inicialização do Personagem
    PERSONAGEM personagem = {0};
    personagem.posicao.width = 25;
    personagem.posicao.height = 25;
    personagem.velocidadeBase = VELOCIDADE_INICIAL;
    personagem.velocidadeAtual = personagem.velocidadeBase;
    personagem.textura = personagemUp;
    personagem.vidas = 3;
    personagem.fase = 0; 
    personagem.tiro.posicao.height = 10;
    personagem.tiro.posicao.width = 10;
    personagem.tiro.velocidade = 10;
    strcpy(personagem.nome, "\0");
  

    //Inicializando PowerUp
    POWERUP powerUp = {0};
    powerUp.posicao.width = 25;
    powerUp.posicao.height = 25;
    powerUp.textura = energyCellTextura;
    do{
        powerUp.posicao.x = GetRandomValue(powerUp.posicao.width, GetScreenWidth()-powerUp.posicao.width);
        powerUp.posicao.y = GetRandomValue(powerUp.posicao.height+40,GetScreenHeight()- powerUp.posicao.height);
    }while(spawnParede(&powerUp, obstaculos, nroBlocos));

    //inicializando recordistas
   // strcpy (recordistas[0].nome, "Bilbo"); 
   // recordistas[0].pontos = 20000; 
   // strcpy (recordistas[1].nome, "Frodo"); 
   // recordistas[1].pontos = 14000; 
   // strcpy (recordistas[2].nome, "Legolas"); 
   // recordistas[2].pontos = 10000; 
   // strcpy (recordistas[3].nome, "Aragorn"); 
   // recordistas[3].pontos = 7000; 
   // strcpy (recordistas[4].nome, "Gandalf"); 
   // recordistas[4].pontos = 5000; 


    FILE *scoresPointer;
    scoresPointer = fopen("../levels/highscores.bin", "rb");

    for (int i = 0; i < 5; i++) {
        fread(&recordistas[i], sizeof(HIGHSCORE), 1, scoresPointer);      //le o arquivo binario para struct
    }
    fclose(scoresPointer);




    SetTargetFPS(60);
    while (!WindowShouldClose()) { // detecta se vai fechar
        // Update
        switch (currentScreen) {
            case MENU:{
                if (auxM == 1) {
                    estadoChave = 1;
                    auxM = 0; 
                }


                if (IsKeyPressed(KEY_DOWN))
                        estadoChave++;
                if (IsKeyPressed(KEY_UP))
                        estadoChave--;

                if (estadoChave == 7)
                        estadoChave = 1;
                if (estadoChave == 0)
                        estadoChave = 6;

                if (estadoChave == 1 && IsKeyPressed(KEY_ENTER))    //novo jogo
                        currentScreen = FASE1; //onde começa     
                if (estadoChave == 2 && IsKeyPressed(KEY_ENTER ))   //continuar 
                        currentScreen = CONTINUAR;
                if (estadoChave == 3 && IsKeyPressed(KEY_ENTER))    //escolher fase
                        currentScreen = ESCOLHAFASE;
                if (estadoChave == 4 && IsKeyPressed(KEY_ENTER))    // sobre
                        currentScreen = SOBRE;
                if (estadoChave == 5 && IsKeyPressed(KEY_ENTER))    //sair
                        currentScreen = SCORES;
                if (estadoChave == 6 && IsKeyPressed(KEY_ENTER))    //sair
                        CloseWindow();
                break;
            }

            case FASE1: {
                
                if (aux) {
                    rewind(Level1p);
                    currentScreen = NOME; 
                    inimigosMortos = 0; 
                    positionX = 0;
                    positionY = 1; 
                    nroBlocos = nroInimigos = 0;
                    maxInimigos = MAX_INIMIGOS;
                    personagem.vidas = 3;
                    personagem.pontos[0] = 0; 
                    personagem.fase = 1; 
                    fimDeJogo = FALSE;
                    recorde = 0; 
                }
            
                criandoMapa (Level1p, &positionX, &positionY, &tipo, obstaculos, &nroBlocos, &personagem);
                criandoInimigos(&inimigosEmTela, tempo, &aux, &nroInimigos, &nroBlocos,&corInimigo, inimigos,inimigoRedDown,inimigoGreenDown,obstaculos,&personagem);
                movendoInimigos (screenHeight, &nroInimigos, &nroBlocos, &colisaoInimigoCenario, &colisaoDoInimigo, inimigos, &personagem,  obstaculos, &corInimigo,  inimigoRedUp,  inimigoRedDown,  inimigoRedLeft,  inimigoRedRight,  inimigoGreenUp,  inimigoGreenDown,  inimigoGreenLeft,  inimigoGreenRight);
                movendoPersonagem (&personagem, &nroBlocos, &nroInimigos,  screenHeight, inimigos, obstaculos,  personagemRight,  personagemLeft,  personagemUp,  personagemDown);

                if (IsKeyPressed(KEY_S)) {
                    salvarJogo(&personagem, inimigos, obstaculos, &nroInimigos, &nroBlocos, &inimigosMortos, &inimigosEmTela, &maxInimigos);
                }

                
                if (personagem.vidas == -1)  {
                    aux = 1; 
                    recorde = personagem.pontos[0]; 
                    salvaScore (recordistas, recorde, &personagem);
                    currentScreen = MENU; 
                }
                    
                if (inimigosMortos >= 15 && inimigosEmTela == 0) {
                    fimDeJogo = TRUE;
                }
                if (fimDeJogo == TRUE)  {    //para passagem de fases
                    if(faseUnica) {
                        faseUnica = FALSE;
                        currentScreen = MENU;
                    }
                    else
                        currentScreen = FASE2;  //falta inserir questão dos scores
                }
                break;
            }

            case FASE2:{
                //lembrar de zerar as variáveis importantes

                if (aux1) {
                    rewind(Level2p);
                    inimigosMortos = 0; 
                    positionX = 0;
                    positionY = 1; 
                    nroBlocos = nroInimigos = 0;
                    maxInimigos = MAX_INIMIGOS;
                    personagem.vidas = 3;
                    fimDeJogo = FALSE;
                    personagem.fase = 2; 
                    personagem.pontos[1] = 0; 
                    recorde = 0; 
                }

                criandoMapa (Level2p, &positionX, &positionY, &tipo, obstaculos, &nroBlocos, &personagem);
                criandoInimigos(&inimigosEmTela,tempo, &aux1, &nroInimigos, &nroBlocos,&corInimigo, inimigos,inimigoRedDown,inimigoGreenDown,obstaculos,&personagem);
                movendoInimigos (screenHeight, &nroInimigos, &nroBlocos, &colisaoInimigoCenario, &colisaoDoInimigo, inimigos, &personagem,  obstaculos, &corInimigo,  inimigoRedUp,  inimigoRedDown,  inimigoRedLeft,  inimigoRedRight,  inimigoGreenUp,  inimigoGreenDown,  inimigoGreenLeft,  inimigoGreenRight);
                movendoPersonagem (&personagem, &nroBlocos, &nroInimigos,  screenHeight, inimigos, obstaculos,  personagemRight,  personagemLeft,  personagemUp,  personagemDown);


                if (IsKeyPressed(KEY_S)) {
                    salvarJogo(&personagem, inimigos, obstaculos, &nroInimigos, &nroBlocos, &inimigosMortos, &inimigosEmTela, &maxInimigos);
                }


                if (personagem.vidas == -1)  {
                    aux1 = 1; 
                    recorde = personagem.pontos[0] + personagem.pontos[1]; 
                    salvaScore (recordistas, recorde, &personagem);
                    currentScreen = MENU; 
                }

                if (inimigosMortos >= 15 && !inimigosEmTela)
                    fimDeJogo = TRUE;

                if (fimDeJogo == TRUE)  {    //para passagem de fases
                    if(faseUnica) {
                        faseUnica = FALSE;
                        currentScreen = MENU;
                    }
                    else
                        currentScreen = FASE3;  //falta inserir questão dos scores
                }
                break;
            }

            case FASE3:{
                //lembrar de zerar as variáveis importantes
                 if (aux2) {
                    rewind(Level3p);
                    inimigosMortos = 0; 
                    positionX = 0;
                    positionY = 1; 
                    nroBlocos = nroInimigos = 0;
                    maxInimigos= MAX_INIMIGOS;
                    fimDeJogo = FALSE;
                    personagem.vidas = 3;
                    personagem.pontos[2] = 0; 
                    personagem.fase = 3; 
                    recorde = 0; 
                }
                
                criandoMapa (Level3p, &positionX, &positionY, &tipo, obstaculos, &nroBlocos, &personagem);
                criandoInimigos(&inimigosEmTela,tempo, &aux2, &nroInimigos, &nroBlocos,&corInimigo, inimigos,inimigoRedDown,inimigoGreenDown,obstaculos,&personagem);
                movendoInimigos (screenHeight, &nroInimigos, &nroBlocos, &colisaoInimigoCenario, &colisaoDoInimigo, inimigos, &personagem,  obstaculos, &corInimigo,  inimigoRedUp,  inimigoRedDown,  inimigoRedLeft,  inimigoRedRight,  inimigoGreenUp,  inimigoGreenDown,  inimigoGreenLeft,  inimigoGreenRight);
                movendoPersonagem (&personagem, &nroBlocos, &nroInimigos,  screenHeight, inimigos, obstaculos,  personagemRight,  personagemLeft,  personagemUp,  personagemDown);


                if (IsKeyPressed(KEY_S)) {
                    salvarJogo(&personagem, inimigos, obstaculos, &nroInimigos, &nroBlocos, &inimigosMortos, &inimigosEmTela, &maxInimigos);
                }


                if (personagem.vidas == -1)  {
                    aux2 = 1; 
                    recorde = personagem.pontos[0] + personagem.pontos[1] + personagem.pontos[2]; 
                    salvaScore (recordistas, recorde, &personagem);
                    currentScreen = MENU; 
                }


                if (inimigosMortos >= 15 && !inimigosEmTela)
                    fimDeJogo = TRUE;

                if (fimDeJogo == TRUE)  {    //para passagem de fases
                    if(faseUnica) {
                        faseUnica = FALSE;
                        currentScreen = MENU;
                    }
                    else
                        currentScreen = FASE4;  //falta inserir questão dos scores
                }
                break;
            }

            case FASE4: {
                
                if (aux3) {
                    rewind(Level4p);
                    inimigosMortos = 0; 
                    positionX = 0;
                    positionY = 1;  
                    nroBlocos = nroInimigos = 0;
                    maxInimigos =MAX_INIMIGOS;
                    fimDeJogo = FALSE;
                    personagem.vidas = 3;
                    personagem.pontos[3] = 0; 
                    personagem.fase = 4; 
                    recorde = 0; 
                }
                //lembrar de zerar as variáveis importantes
                

                criandoMapa (Level4p, &positionX, &positionY, &tipo, obstaculos, &nroBlocos, &personagem);
                criandoInimigos(&inimigosEmTela,tempo, &aux3, &nroInimigos, &nroBlocos,&corInimigo, inimigos,inimigoRedDown,inimigoGreenDown,obstaculos,&personagem);
                movendoInimigos (screenHeight, &nroInimigos, &nroBlocos, &colisaoInimigoCenario, &colisaoDoInimigo, inimigos, &personagem,  obstaculos, &corInimigo,  inimigoRedUp,  inimigoRedDown,  inimigoRedLeft,  inimigoRedRight,  inimigoGreenUp,  inimigoGreenDown,  inimigoGreenLeft,  inimigoGreenRight);
                movendoPersonagem (&personagem, &nroBlocos, &nroInimigos,  screenHeight, inimigos, obstaculos,  personagemRight,  personagemLeft,  personagemUp,  personagemDown);


                if (IsKeyPressed(KEY_S)) {
                    salvarJogo(&personagem, inimigos, obstaculos, &nroInimigos, &nroBlocos, &inimigosMortos, &inimigosEmTela, &maxInimigos);
                }


                if (personagem.vidas == -1)  {
                    aux3 = 1; 
                    recorde = personagem.pontos[0] + personagem.pontos[1] + personagem.pontos[2] + personagem.pontos[3]; 
                    salvaScore (recordistas, recorde, &personagem);
                    currentScreen = MENU; 
                }


                if (inimigosMortos >= 15 && !inimigosEmTela)
                    fimDeJogo = TRUE;

                if (fimDeJogo == TRUE)  {    //para passagem de fases
                    if(faseUnica) {
                        faseUnica = FALSE;
                        currentScreen = MENU;
                    }
                    else
                        currentScreen = SOBRE;  //falta inserir questão dos scores
                }
                break;

            }

            case CONTINUAR: {
            
                if (auxC) {
                    inimigosMortos = 0;  
                    nroBlocos = nroInimigos = 0;
                    maxInimigos =MAX_INIMIGOS;
                    fimDeJogo = FALSE;
                    personagem.posicao.x = 0;
                    personagem.posicao.y = 0;
                    lerJogo(&personagem,inimigos, obstaculos, &nroInimigos, &nroBlocos, &inimigosMortos, &inimigosEmTela, &maxInimigos); 
                    recorde = 0; 
                }

                Rectangle auxPersonagem;
                auxPersonagem.x = personagem.posicao.x;
                auxPersonagem.y = personagem.posicao.y;
               //ler jogo antigo
                
                criandoMapa (savePointer, &positionX, &positionY, &tipo, obstaculos, &nroBlocos, &personagem);
                if (auxC) {
                    personagem.posicao.x = auxPersonagem.x;
                    personagem.posicao.y = auxPersonagem.y; 
                }
                criandoInimigos(&inimigosEmTela,tempo, &auxC, &nroInimigos, &nroBlocos,&corInimigo, inimigos,inimigoRedDown,inimigoGreenDown,obstaculos,&personagem);
                movendoInimigos (screenHeight, &nroInimigos, &nroBlocos, &colisaoInimigoCenario, &colisaoDoInimigo, inimigos, &personagem,  obstaculos, &corInimigo,  inimigoRedUp,  inimigoRedDown,  inimigoRedLeft,  inimigoRedRight,  inimigoGreenUp,  inimigoGreenDown,  inimigoGreenLeft,  inimigoGreenRight);
                movendoPersonagem (&personagem, &nroBlocos, &nroInimigos,  screenHeight, inimigos, obstaculos,  personagemRight,  personagemLeft,  personagemUp,  personagemDown);

                if (IsKeyPressed(KEY_S)) {
                    salvarJogo(&personagem, inimigos, obstaculos, &nroInimigos, &nroBlocos, &inimigosMortos, &inimigosEmTela, &maxInimigos);
                }


                
                if (personagem.vidas == -1)  {
                    auxC = 1; 
                    switch (personagem.fase) {
                        case 1:  recorde = personagem.pontos[0]; break;
                        case 2:  recorde = personagem.pontos[0] + personagem.pontos[1]; break;
                        case 3:  recorde = personagem.pontos[0] + personagem.pontos[1] + personagem.pontos[2]; break;
                        case 4:  recorde = personagem.pontos[0] + personagem.pontos[1] + personagem.pontos[2] + personagem.pontos[3]; break;
                        default: break;
                        salvaScore (recordistas, recorde, &personagem);
                    }
                    currentScreen = MENU; 
                }

                if (inimigosMortos >= 15 && !inimigosEmTela)
                    fimDeJogo = TRUE;

                if (fimDeJogo == TRUE)  {    //para passagem de fases
                    if(faseUnica) {
                        faseUnica = FALSE;
                        currentScreen = MENU;
                    }
                    else {
                        switch (personagem.fase)  {
                            case 1: 
                                currentScreen = FASE2;
                                break;
                            case 2: 
                                currentScreen = FASE3; 
                                break;
                            case 3: 
                                currentScreen = FASE4;
                                break;
                            case 4: 
                                currentScreen = SOBRE; 
                                break;
                        }
                    }
                }

                break;
            }

            case SOBRE: {
                 if (auxSobre) {
                    rewind(sobreP);
                    positionX = 0;
                    positionY = 1;  
                    nroBlocos= 0;
                    auxSobre = 0; 
                }
                criandoMapa (sobreP, &positionX, &positionY, &tipo, obstaculos, &nroBlocos, &personagem);
                movendoPersonagem (&personagem, &nroBlocos, &nroInimigos,  screenHeight, inimigos, obstaculos,  personagemRight,  personagemLeft,  personagemUp,  personagemDown);

                if (IsKeyPressed(KEY_DELETE)){
                    currentScreen = MENU; 
                    auxSobre = 1; 
                }
                break;
            }

            case ESCOLHAFASE: {

                if (IsKeyPressed(KEY_DELETE)){
                    currentScreen = MENU; 
                }

                if (IsKeyPressed(KEY_DOWN))
                        estadoChaveFase++;
                if (IsKeyPressed(KEY_UP))
                        estadoChaveFase--;

                if (estadoChaveFase == 5)
                        estadoChaveFase = 1;
                if (estadoChaveFase == 0)
                        estadoChaveFase = 4;

                faseUnica = TRUE; 
                if (estadoChaveFase == 1 && IsKeyPressed(KEY_ENTER))    
                        currentScreen = FASE1;     
                if (estadoChaveFase == 2 && IsKeyPressed(KEY_ENTER ))    
                        currentScreen = FASE2;
                if (estadoChaveFase == 3 && IsKeyPressed(KEY_ENTER))    
                        currentScreen = FASE3;
                if (estadoChaveFase == 4 && IsKeyPressed(KEY_ENTER))    
                        currentScreen = FASE4;

                break;
            }
            case SCORES: {
                if (IsKeyPressed(KEY_DELETE)){
                    currentScreen = MENU; 
                }
                break;
            }
            case NOME: {
              // Update
                //----------------------------------------------------------------------------------
                if (CheckCollisionPointRec(GetMousePosition(), textBox)) mouseOnText = true;
                else mouseOnText = false;

                if (mouseOnText)
                {
                    // Set the window's cursor to the I-Beam
                    SetMouseCursor(MOUSE_CURSOR_IBEAM);

                    // Get char pressed (unicode character) on the queue
                    int key = GetCharPressed();

                    // Check if more characters have been pressed on the same frame
                    while (key > 0)
                    {
                        // NOTE: Only allow keys in range [32..125]
                        if ((key >= 32) && (key <= 125) && (letterCount < 18))
                        {
                            personagem.nome[letterCount] = (char)key;
                            personagem.nome[letterCount+1] = '\0'; // Add null terminator at the end of the string.
                            letterCount++;
                        }

                        key = GetCharPressed();  // Check next character in the queue
                    }

                    if (IsKeyPressed(KEY_BACKSPACE))
                    {
                        letterCount--;
                        if (letterCount < 0) letterCount = 0;
                        personagem.nome[letterCount] = '\0';
                    }
                }
                else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

                if (mouseOnText) framesCounter++;
                else framesCounter = 0;

                if (IsKeyPressed(KEY_ENTER)) 
                    currentScreen = FASE1;
                    
                

                //----------------------------------------------------------------------------------
            }
        
            
            default: break;
        }

        // Draw
        BeginDrawing();
            ClearBackground(RAYWHITE);
            switch (currentScreen) {
                case MENU: {
                    DrawRectangle(0, 0, screenWidth, screenHeight, BLACK);
                    DrawTexture(tituloTex, 0, 20 , WHITE);
                    u = 0; //escreve menu
                    for (int i = 0; i < 6; i++) {
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
                        case 4: DrawTextEx(arcade, menuOptions[3], (Vector2){(alignCenterFont(menuOptions[3],2,40, arcade)),(400) }, 40, 1,YELLOW);
                            break;
                        case 5: DrawTextEx(arcade, menuOptions[4], (Vector2){(alignCenterFont(menuOptions[4],2,40, arcade)),(450) }, 40, 1,YELLOW);
                            break;
                        case 6: DrawTextEx(arcade, menuOptions[5], (Vector2){(alignCenterFont(menuOptions[5],2,40, arcade)),(500) }, 40, 1,YELLOW);
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
                        if(obstaculos[j].destruido ==TRUE)
                            explodeObstaculo(&obstaculos[j], expObst1, expObst2, expObst3, expObst4, expObst5, expObst6, expObst7);
                    }

                    administraPowerUp(&powerUp, &personagem, obstaculos, nroBlocos, screenHeight, screenWidth);
                    administraTiro(&personagem, screenWidth, screenHeight, obstaculos , nroBlocos,  inimigos, nroInimigos, &maxInimigos, &inimigosMortos, &inimigosEmTela);
                    DrawTexture(personagem.textura, (personagem.posicao.x ), (personagem.posicao.y ), RAYWHITE);
                    desenhaCabecalho(&personagem, escudoTextura, arcade, fase[0]);
                        
                    //desenhando inimigos na tela
                    for (int i = 0; i<nroInimigos; i++) {

                        if(inimigos[i].vivo==FALSE){
                            DrawTexture(inimigoDead, (inimigos[i].posicao.x),  (inimigos[i].posicao.y), RAYWHITE);
                            explodeInimigo(&inimigos[i], explosaoVerde2, explosaoVerde3, explosaoVerde4, explosaoVerde5, explosaoVerde6, explosaoVerde7);
                        } //testando

                        else {
                            administraTiroInimigos( &inimigos[i],  screenWidth, screenHeight, obstaculos,  nroBlocos,  &personagem);                                DrawTexture(inimigos[i].textura, (inimigos[i].posicao.x),  (inimigos[i].posicao.y), RAYWHITE);
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
                    desenhaCabecalho(&personagem, escudoTextura, arcade, fase[1]);

                    //desenhando inimigos na tela
                    for (int i = 0; i<nroInimigos; i++) {
                        if(inimigos[i].vivo==TRUE)
                            DrawTexture(inimigos[i].textura, (inimigos[i].posicao.x),  (inimigos[i].posicao.y), RAYWHITE);
                        if(inimigos[i].vivo==FALSE){
                                DrawTexture(inimigoDead, (inimigos[i].posicao.x),  (inimigos[i].posicao.y), RAYWHITE);
                                explodeInimigo(&inimigos[i], explosaoVerde2, explosaoVerde3, explosaoVerde4, explosaoVerde5, explosaoVerde6, explosaoVerde7);
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
                    desenhaCabecalho(&personagem, escudoTextura, arcade, fase[2]);

                    //desenhando inimigos na tela
                    for (int i = 0; i<nroInimigos; i++) {
                        if(inimigos[i].vivo==TRUE)
                            DrawTexture(inimigos[i].textura, (inimigos[i].posicao.x),  (inimigos[i].posicao.y), RAYWHITE);
                        if(inimigos[i].vivo==FALSE)
                            DrawTexture(inimigoDead, (inimigos[i].posicao.x),  (inimigos[i].posicao.y), RAYWHITE);

                        //Tiro dos inimigos
                        administraTiroInimigos( &inimigos[i], screenWidth, screenHeight, obstaculos, nroBlocos, &personagem);



                    }
                    break;
                }

                case FASE4: {
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
                    desenhaCabecalho(&personagem, escudoTextura, arcade, fase[3]);

                    //desenhando inimigos na tela
                    for (int i = 0; i<nroInimigos; i++) {
                        if(inimigos[i].vivo==TRUE)
                            DrawTexture(inimigos[i].textura, (inimigos[i].posicao.x),  (inimigos[i].posicao.y), RAYWHITE);
                        if(inimigos[i].vivo==FALSE)
                            DrawTexture(inimigoDead, (inimigos[i].posicao.x),  (inimigos[i].posicao.y), RAYWHITE);

                        //Tiro dos inimigos
                        administraTiroInimigos( &inimigos[i], screenWidth, screenHeight, obstaculos, nroBlocos, &personagem);

                    }
                    break;
                }

                case CONTINUAR:{
                    
                    ClearBackground(RAYWHITE);
                    DrawRectangle(0,0,screenWidth,screenHeight,BLACK);

                    for (int j = 0; j < nroBlocos; j++) {
                        int xvec = obstaculos[j].posicao.x;
                        int yvec =  obstaculos[j].posicao.y;
                        if (!obstaculos[j].destruido)
                            DrawTexture(brickTexture, xvec, yvec, WHITE);
                    }

                    administraPowerUp(&powerUp, &personagem, obstaculos, nroBlocos, screenHeight, screenWidth);
                    administraTiro(&personagem, screenWidth, screenHeight, obstaculos , nroBlocos,  inimigos, nroInimigos, &maxInimigos, &inimigosMortos, &inimigosEmTela);
                    DrawTexture(personagem.textura, (personagem.posicao.x ), (personagem.posicao.y ), RAYWHITE);
                    desenhaCabecalho(&personagem, escudoTextura, arcade, "Continuar");

                    //desenhando inimigos na tela
                    for (int i = 0; i<nroInimigos; i++) {
                        if(inimigos[i].vivo==FALSE) //testando
                            DrawTexture(inimigoDead, (inimigos[i].posicao.x),  (inimigos[i].posicao.y), RAYWHITE);
                        else {
                            administraTiroInimigos( &inimigos[i],  screenWidth, screenHeight, obstaculos,  nroBlocos,  &personagem);
                            DrawTexture(inimigos[i].textura, (inimigos[i].posicao.x),  (inimigos[i].posicao.y), RAYWHITE);
                        }
                    }

                    break;
                }

                case SOBRE: {
                    ClearBackground(RAYWHITE);
                    DrawRectangle(0, 0, screenWidth, screenHeight, BLACK);
                    DrawRectangle(0, 0, screenWidth, 10, WHITE);
                    DrawTextEx(arcade, "Pressione Delete para voltar ao menu", (Vector2){10,10 }, 15, 1,WHITE);
                    DrawTextEx(arcade, "TRABALHO FINAL DE ALGORTIMOS", (Vector2){(alignCenterFont("TRABALHO FINAL DE ALGORTIMOS",0,50, arcade)),200 }, 50, 1,WHITE);
                    DrawTextEx(arcade, "E PROGRAMACAO", (Vector2){(alignCenterFont("E PROGRAMACAO",0,50, arcade)),240 }, 50, 1,WHITE);

                    DrawTextEx(arcade, "Rafael Morschbacher", (Vector2){(alignCenterFont("Rafael Morschbacher",0,30, arcade)),350 }, 30, 1,WHITE);
                    DrawTextEx(arcade, "Juliana de Vargas", (Vector2){(alignCenterFont("Juliana de Vargas",0,30, arcade)),380 }, 30, 1,WHITE);
                    
                    for(int i = 0; i < 40; i++){
                        int xvec = i*25; 
                         DrawTexture(brickTexture, xvec, 10, WHITE);
                    }
                    

                    for (int j = 0; j < nroBlocos; j++ ) {
                        int xvec = obstaculos[j].posicao.x;
                        int yvec =  obstaculos[j].posicao.y;
                        if (!obstaculos[j].destruido)
                            DrawTexture(brickTexture, xvec, yvec, WHITE);
                    }
                    administraTiro(&personagem, screenWidth, screenHeight, obstaculos , nroBlocos,  inimigos, nroInimigos, &maxInimigos, &inimigosMortos, &inimigosEmTela);
                    DrawTexture(personagem.textura, (personagem.posicao.x ), (personagem.posicao.y ), RAYWHITE);

                    DrawTextEx(arcade, "Pressione Delete para voltar ao menu", (Vector2){10,10 }, 15, 1,WHITE);
                    
                    break;
                }

                case ESCOLHAFASE: {
                    ClearBackground(RAYWHITE);
                    DrawRectangle(0, 0, screenWidth, screenHeight, BLACK);
                    DrawTextEx(arcade, "Pressione Delete para voltar ao menu", (Vector2){10,20 }, 15, 1,WHITE);
                    DrawRectangle(100, 50, 800, 550, GRAY);
                    DrawRectangle(125, 75, 750, 500, BLACK);
                    


                     u = 0; //escreve menu
                    for (int i = 0; i < 4; i++) {
                        DrawTextEx(arcade, fase[i], (Vector2){(alignCenterFont(fase[i],i,40, arcade)),(250 + u) }, 40, 1,WHITE);
                        u+= 50.0;
                    }

                    switch (estadoChaveFase) {//deixa amarelo
                        case 1: DrawTextEx(arcade, fase[0], (Vector2){(alignCenterFont(fase[0],0,40, arcade)),250 }, 40, 1,YELLOW);
                            break;
                        case 2: DrawTextEx(arcade, fase[1], (Vector2){(alignCenterFont(fase[1],1,40, arcade)),(300) }, 40, 1,YELLOW);
                            break;
                        case 3: DrawTextEx(arcade, fase[2], (Vector2){(alignCenterFont(fase[2],2,40, arcade)),(350) }, 40, 1,YELLOW);
                            break;
                        case 4: DrawTextEx(arcade, fase[3], (Vector2){(alignCenterFont(fase[3],2,40, arcade)),(400) }, 40, 1,YELLOW);
                            break;
                        
                        default: break;
                    }
                    break;
                }
                case SCORES: {
                    
                    ClearBackground(RAYWHITE);
                    DrawRectangle(0, 0, screenWidth, screenHeight, BLACK);
                    DrawTextEx(arcade, "Pressione Delete para voltar ao menu", (Vector2){10,10 }, 15, 1,WHITE);
                    DrawTextEx(arcade, "HIGHSCORES", (Vector2){(alignCenterFont("HIGHSCORES",0,70, arcade)),(100) }, 70, 1,WHITE);

                    u = 0; //escreve menu
                    for (int i = 0; i < 5; i++) {
                        DrawTextEx(arcade, recordistas[i].nome, (Vector2){200,(250 + u) }, 40, 1,WHITE);
                        u+= 50.0;
                    }
                    char score[20];

                    u = 0; //escreve menu
                    for (int i = 0; i < 5; i++) {
                        sprintf(score, "%i", recordistas[i].pontos);
                        DrawTextEx(arcade, score, (Vector2){675,(250 + u) }, 40, 1,WHITE);
                        u+= 50.0;
                    }
                    break;
                }
                case NOME: {

                    ClearBackground(RAYWHITE);
                    DrawRectangle(0, 0, screenWidth, screenHeight, BLACK);

                    DrawTextEx(arcade, "coloque o mouse em cima da caixa!", (Vector2) {alignCenterFont("coloque o mouse em cima da caixa!", 0, 20, arcade), 260}, 20, 1, WHITE);

                    DrawRectangleRec(textBox, LIGHTGRAY);
                    if (mouseOnText) DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, YELLOW);
                    else DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, GRAY);

                    //DrawText(name, (int)textBox.x + 5, (int)textBox.y + 8, 40, BLACK);
                    DrawTextEx(arcade,personagem.nome, (Vector2) {(int)textBox.x + 5, (int)textBox.y + 8}, 40, 1, DARKGRAY);


                    DrawTextEx(arcade, TextFormat("caracteres %i de %i", letterCount, 18), (Vector2) {alignCenterFont("caracteres 10 de 10", 0, 20, arcade), 375}, 20, 1, WHITE);


                    if (mouseOnText)
                    {
                        if (letterCount < 18)
                        {
                            // Draw blinking underscore char
                            if (((framesCounter/18)%2) == 0) DrawText("_", (int)textBox.x + 8 + MeasureText(personagem.nome, 40), (int)textBox.y + 12, 40, YELLOW);
                        }
                        else DrawTextEx(arcade, "pressione backspace para apagar", (Vector2) {alignCenterFont("pressione backspace para apagar", 0, 15, arcade), 410}, 15, 1, WHITE);

                    }
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
    fclose (savePointer); 
    fclose (sobreP); 

    CloseWindow();
    return 0;
}

