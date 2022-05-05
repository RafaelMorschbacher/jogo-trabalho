# jogo-trabalho
Trabalho final de algoritmos e programação: jogo BATTLE INF. <br>
Linguagem: C <br>
Biblioteca utilizada: Raylib<br>
Breve descrição do trabalho e das structs:<br>
O jogo battle inf é um jogo de batalha entre tanques de guerra, com cenário destrutível pelo player e pelos inimigos. Os inimigos spawnam aleatóriamente, sendo no máximo 5 inimigos em tela por vez. <br>
O jogador acumula pontos ao destruir tanques inimigos e perde pontos ao explodir paredes do cenário. Além disso, o jogador pode coletar power-ups que spawnam aleatóriamente pelo cenário para aumentar sua velocidade por alguns segundos.<br>
O jogo conta com 4 fases, e o jogador pode salvar sua pontuação para competir com outros jogadores. Os 5 melhores jogadores tem suas pontuações exibidas em um scoreboard.<br>

As estruturas utilizadas no jogo são as seguintes:<br>

OBSTACULO: <br>
  Rectangle posicao: é um dado do tipo Rectangle, que indica as dimensões e a posição dos blocos no mapa. <br>
  int destruido: Booleano (0 ou 1) que indica se o player ja destruiu a parede em questão. Nesse caso, ela não aparece mais no mapa.<br>
  int contadorAnimacao: Contador auxiliar para realizar a troca dos frames da animação de explosão, só é incrementada quando o obstáculo é explodido.<br>

PERSONAGEM: (player)<br>
  Rectangle posicao: posicao e dimensões do personagem<br>
  Texture2D textura<br>
  int velocidadeBase: velocidade com que o personagem spawna<br>
  int velocidadeAtual: velocidade que pode ser a base ou incrementada (pelo powerup)<br>
  int inclinacao: para qual direcao o personagem esta virado (cima, baixo, esq, dir)<br>
  int vidas: Número de tiros que o personagem ainda pode tomar<br>
  int pontos[4]: cada item do array é uma fase (pontuação)<br>
  int fase: fase atual que está sendo jogada<br>
  char nome[20]: nickname do player<br>
  TIRO tiro: atributos do tiro do player<br>
  
INIMIGO:<br>
    char modo: pode ser Patrulha (movimentos aleatórios em retas) ou Perseguição (persegue o personagem)<br>
    char sentidoDesloc: Sentido de deslocamento (Aleatorio ou seguindo personagem)<br>
    Rectangle posicao: Posição do inimigo<br>
    Texture2D textura;<br>
    float velocidade;<br>
    int orientacao: Se o inimigo está indo para cima baixo, esq ou dir<br>
    int colisao: TRUE (1) ou FALSE (0) para checao colisao com outros personagens ou obstaculos<br>
    char cor: inimigos vermelhos ou verdes (variação estética)<br>
    int vivo: Booleano se está vivo ou se já foi abatido pelo jogador<br>
    TIRO tiro: atributos do tiro do player<br>
    int contadorAnimacao: Contador auxiliar para realizar a troca dos frames da animação de explosão, só é incrementada quando o inimigo é abatido.<br>
    
POWER-UP:<br>
    Rectangle posicao: Posição e dimensões do powerup<br>
    int ativo: Se o efeito do powerup está ativado<br>
    int cooldown: intervalo após a utilização até ele respawnar em outro ponto aleatório do mapa.<br>
    Texture2D textura;<br>
    
TIRO: (mesma struct usada pelo personagem e pelos inimigos)<br>
    Rectangle posicao;<br>
    Texture2D textura;<br>
    int atirando: Booleano, se o tiro está em movimento, já foi disparado e ainda não atingiu nada.<br>
    int velocidade;<br>
    int inclinacao: Se o tiro está indo para cima baixo, esq ou dir<br>
    
 HIGHSCORE: (usada para guardar a pontuação de um jogador no arquivo de high scores)<br>
    int pontos;<br>
    char nome[20];<br>


COMO ABRIR?
VS CODE: Abrir a pasta jogo-trabalho, e rodar utilizando o MAKE FILE
CODEBLOCS: Criar projeto com .h e .c. 
