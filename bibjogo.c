#include <stdio.h>
#include <raylib.h>
#include <string.h>
#include "bibjogo.h"

// FUNÇÕES GERAIS ------------------------------------------------------------------------------------------------------------------
float alignCenterFont (char *v, int i, int fontSize, Font arcade) { 
    float width;
    Vector2 tamText = MeasureTextEx(arcade, v, fontSize, 1);
    width = ((screenWidth-tamText.x)/2);
    return width;

     printf("alinhando center font \n"); 
}

// FUNÇÕES MENU --------------------------------------------------------------------------------------------------------------------
/*int posicaoChave() {                //da posição da chave, pra saber onde deve estar amarelo
    int estadoChave = 1;
    if (IsKeyPressed(KEY_DOWN))
        estadoChave++;
    if (IsKeyPressed(KEY_UP))
        estadoChave--;
    if (estadoChave == 4)
        estadoChave = 1;
    if (estadoChave == 0)
        estadoChave = 3;

    printf("estado chave = %i\n", estadoChave); 
    return estadoChave; 
}
*/
// FUNÇOES NOVO JOGO ----------------------------------------------------------------------------------------------------------------
// -- carregando mapa
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

    printf("lendo arquivo, x = %i, y = %i\n", xFuncao, yFuncao); 
    return feof(level);
}

