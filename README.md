# jogo-trabalho
Trabalho final de algoritmos e programação: jogo BATTLE INF. <br>
Linguagem: C
Biblioteca utilizada: Raylib
Breve descrição do trabalho e das structs:
O jogo battle inf é um jogo de batalha entre tanques de guerra, com cenário destrutível pelo player e pelos inimigos. Os inimigos spawnam aleatóriamente, sendo no máximo 5 inimigos em tela por vez. 
O jogador acumula pontos ao destruir tanques inimigos e perde pontos ao explodir paredes do cenário. Além disso, o jogador pode coletar power-ups que spawnam aleatóriamente pelo cenário para aumentar sua velocidade por alguns segundos.
O jogo conta com 4 fases, e o jogador pode salvar sua pontuação para competir com outros jogadores. Os 5 melhores jogadores tem suas pontuações exibidas em um scoreboard.

As estruturas utilizadas no jogo são as seguintes:

OBSTACULO: 
  Rectangle posicao: é um dado do tipo Rectangle, que indica as dimensões e a posição dos blocos no mapa. 
  int destruido: Booleano (0 ou 1) que indica se o player ja destruiu a parede em questão. Nesse caso, ela não aparece mais no mapa.
  int contadorAnimacao: Contador auxiliar para realizar a troca dos frames da animação de explosão, só é incrementada quando o obstáculo é explodido.

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
  
INIMIGO:
    char modo: pode ser Patrulha (movimentos aleatórios em retas) ou Perseguição (persegue o personagem)
    char sentidoDesloc: Sentido de deslocamento (Aleatorio ou seguindo personagem)
    Rectangle posicao: Posição do inimigo
    Texture2D textura;
    float velocidade;
    int orientacao: Se o inimigo está indo para cima baixo, esq ou dir
    int colisao: TRUE (1) ou FALSE (0) para checao colisao com outros personagens ou obstaculos
    char cor: inimigos vermelhos ou verdes (variação estética)
    int vivo: Booleano se está vivo ou se já foi abatido pelo jogador
    TIRO tiro: atributos do tiro do player
    int contadorAnimacao: Contador auxiliar para realizar a troca dos frames da animação de explosão, só é incrementada quando o inimigo é abatido.
    
POWER-UP:
    Rectangle posicao: Posição e dimensões do powerup
    int ativo: Se o efeito do powerup está ativado
    int cooldown: intervalo após a utilização até ele respawnar em outro ponto aleatório do mapa.
    Texture2D textura;
    
TIRO: (mesma struct usada pelo personagem e pelos inimigos)
    Rectangle posicao;
    Texture2D textura;
    int atirando: Booleano, se o tiro está em movimento, já foi disparado e ainda não atingiu nada.
    int velocidade;
    int inclinacao: Se o tiro está indo para cima baixo, esq ou dir
    
 HIGHSCORE: (usada para guardar a pontuação de um jogador no arquivo de high scores)
    int pontos;
    char nome[20];
