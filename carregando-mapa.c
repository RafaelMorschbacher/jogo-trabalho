/*
esse programa lê um arquivo de texto com o mapa, e a partir disso, cria um array com as posições dos tijolos e do jogador
*/

#include <stdio.h>
#include <raylib.h>


#define screenWidth 1000

#define screenHeight 640

int readLevel (FILE *level, int *positionX, int *positionY, char *tipo) { // função que le arquivo, 1 caractere por vez, e descobre qual é
    int xFuncao = *positionX;
    int yFuncao = *positionY;
    char conteudoTipo = *tipo;
    char leitura;
    do {
        fscanf(level,"%c",&leitura);
        switch(leitura) {

          case '\n':leitura = '-'; // para não ser contabilizados os ENTER do arquivo
                    break;
          case '-': xFuncao++;
                    break;
          case 'T': xFuncao++;
                    conteudoTipo = 'T';
                    break;
          case '#': xFuncao++;
                    conteudoTipo = '#';
                    break;
          default:  break;
        }
        if (xFuncao == 41) {
            xFuncao = 1;
            yFuncao++;
        }

    } while (leitura == '-' && (feof(level) == 0) ); // sai da função quando leitura não é mais '-'. enquanto for, continua lendo
                                                     // só vai sair no caso de # ou T. a funçao só para nesses dois casos, e a execução retorna pro main
    
    *positionX = xFuncao;  //muda a coordenada x do prox objeto
    *positionY = yFuncao;  //muda a coordenada y do prox objeto
    *tipo = conteudoTipo;
    return feof(level);
}

int main(void) {
    // variaveis
    int positionX = 0; //coordenada x de um objeto na tela
    int positionY = 1; //coordenada y de um objeto na tela
    int brickWall[600][2] = {0};
    int positionPlayer[1][2] = {0};
    int nroBlocos = 0;
    char tipo = ' ';

    //init
    InitWindow(screenWidth, screenHeight, "Nome da Janela");

    //Arquivo 
    FILE *fileLevel;
    fileLevel = fopen("../levels/nivel1.txt", "r");

    //Load
    Image brick = LoadImage("../assets/brick_texture2.png");
    ImageResize(&brick, 25, 40);
    Texture2D brickTexture = LoadTextureFromImage(brick);

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
    while (!WindowShouldClose()) {

        // Update

        // Draw
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawRectangle(0,0,screenWidth,screenHeight,BLACK);
            DrawRectangle(0,0,screenWidth,40,GRAY);

            for (int j = 0; j < nroBlocos; j++ ) {
                int xvec = ((brickWall[j][0]-1)*25);
                int yvec = ((brickWall[j][1]-1)*40)+40;
                DrawTexture(brickTexture, xvec, yvec, WHITE);
            }

        EndDrawing();
        //----------------------------------------------------------------------------------

    }
    //Unload
    fclose (fileLevel);
    CloseWindow();
    return 0;
}
