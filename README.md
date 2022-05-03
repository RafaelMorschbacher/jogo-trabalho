# jogo-trabalho
Trabalho final de algoritmos e programação: jogo BATTLE INF.
Linguagem: C
Biblioteca utilizada: Raylib
Breve descrição do trabalho e das structs:
O jogo battle inf é um jogo de batalha entre tanques de guerra, com cenário destrutível pelo player e pelos inimigos. Os inimigos spawnam aleatóriamente, sendo no máximo 5 inimigos em tela por vez. O jogador acumula pontos ao destruir tanques inimigos e perde pontos ao explodir paredes do cenário. O jogo conta com 4 fases, e o jogador pode salvar sua pontuação para competir com outros jogadores. Os 5 melhores jogadores tem suas pontuações exibidas em um scoreboard.
As estruturas utilizadas no jogo são as seguintes:

OBSTACULO: 
  Rectangle posicao: é um dado do tipo Rectangle, que indica as dimensões e a posição dos blocos no mapa. 
  int destruido: que indica se o player ja destruiu a parede em questão. Nesse caso, ela não aparece mais no mapa.

PERSONAGEM: (player)
  Rectangle posicao: posicao e dimensões do personagem
  Texture2D textura
  int velocidadeBase: velocidade com que o personagem spawna
  int velocidadeAtual: velocidade que pode ser a base ou incrementada (pelo powerup)
  int inclinacao: para qual direcao o personagem esta virado (cima, baixo, esq, dir)
  int vidas: Número de tiros que o personagem ainda pode tomar
  int pontos[4]: cada item do array é uma fase (pontuação)
  int fase: fase atual que está sendo jogada
  char nome[20]: nickname do player
  TIRO tiro: atributos do tiro do player

