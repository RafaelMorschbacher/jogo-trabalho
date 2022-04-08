#include <stdio.h>
#include <raylib.h>


int readLevel (FILE *level, int *x, int *y, char *tipo) {

    int conteudoX = *x;
    int conteudoY = *y;
    char conteudoTipo = *tipo;
    char leitura;

    do {
        fscanf(level,"%c",&leitura);
        switch(leitura) {

          case '\n':leitura = '-';
                    break;
          case '-': conteudoX++;
                    conteudoTipo = '-';
                    break;
          case 'T': conteudoX++;
                    conteudoTipo = 'T';
                    break;
          case '#': conteudoX++;
                    break;
          default:  break;
        }
        if (conteudoX == 41) {
            conteudoX = 1;
            conteudoY++;
        }

    } while (leitura == '-' && (feof(level) == 0) );


  // sai da função quando leitura não é mais '-'

    *x = conteudoX;
    *y = conteudoY;
    *tipo = conteudoTipo;

   // printf("feof %i", feof(level));
    return feof(level);


}


int main(void) {
    // variaveis
    const int screenWidth = 1040;
    const int screenHeight = 600;
    int positionX = 0;
    int positionY = 1;
    int brickWall[600][2] = {0};
    int i = 0;
    char tipo = ' ';
    int positionPlayer[1][2];

    //init
    InitWindow(screenWidth, screenHeight, "Nome da Janela");

    //Load

    Image brick = LoadImage("assets/brick_texture2.png");
    ImageResize(&brick, 25, 40);
    Texture2D brickTexture = LoadTextureFromImage(brick);

    FILE *fileLevel;
    fileLevel = fopen("levels/nivel1.txt", "r");

    while (readLevel(fileLevel, &positionX, &positionY,&tipo) == 0) {
        printf ("posicao x: %i, posicao y: %i\n", positionX, positionY);

        if (tipo == '-') {
            brickWall[i][0] = positionX;
            brickWall[i][1] = positionY;
            i++;
        }
        else if (tipo == 'T') {
            positionPlayer[0][0] = positionX;
            positionPlayer[0][1] = positionY;
            printf ("entrei no tipo T");
        }
    }
    // teste printando posições
    for ( int j = 0; j < i; j++) {
        printf("bloco numero %i, x = %i; y = %i\n",j, brickWall[j][0], brickWall[j][1]);
    }
    printf("posicao player = %i, %i", positionPlayer[0][0], positionPlayer[0][1]);
    // fim do teste

    SetTargetFPS(60);
    while (!WindowShouldClose()) {

        // Update

        // Draw
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawRectangle(0,0,screenWidth,screenHeight,BLACK);
            DrawRectangle(0,0,screenWidth,40,GRAY);

            for (int j = 0; j < i; j++ ) {
                int xvec = ((brickWall[j][0]-1)*25);
                int yvec = ((brickWall[j][1]-1)*40);
                //DrawRectangle(xvec,yvec,25,40,ORANGE);

                DrawTexture(brickTexture, xvec, yvec, WHITE);
                //printf("vecs %i, %i\n", xvec,yvec);
                //printf (" j vale %i, i vale %i\n", j, i);
                //printf ("coor x = %i, coor y = %i\n",brickWall[i][0],brickWall[i][1] );
            }

        EndDrawing();
        //----------------------------------------------------------------------------------

    }


    //Unload
    fclose (fileLevel);
    CloseWindow();
    return 0;
}
