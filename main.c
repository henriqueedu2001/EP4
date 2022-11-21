#include <stdio.h>
#include <stdlib.h>

/* DOCUMENTAÇÃO */

/* ALGORITMO */
/* Inicialmente, pensei em implantar um minimax. No entanto, percebi que sua complexidade é 
simplesmente catastrófica; para escolher a primeira jogada, ele precisa simular todas as
possíveis jogadas para cada uma das n^2 posições. Após certa pesquisa, vi que era um resul-
-tado relativamente conhecido de teoria dos jogos que a estratégia de obstruir a maior quan-
-tidade de vias possíveis enquanto busca a maior quantidade de alinhamentos favoráveis é a
melhor técnica para o jogo da velha n x n. Eu conjecturo que esta seja a melhor técnica pa-
-ra no n x n x n 3D e baseio minha estratégia nela.
  Assim, temos o seguinte algoritmo:
    (i) para cada posição (x,y), calcular uma pontuação
    (ii) a pontuação é calculada a partir da quantidade de linhas retas que aquela jogada é
    capaz de bloquear ou preencher.
    (iii) escolher a posição com a maior pontuação.
  A pontuação segue aos seguintes critérios: para cada linha que será obstruída, ou seja, 
inutilizada para o adversário, acrescentar uma unidade à pontuação. Para cada linha prestes
a ser preenchida pelo adversário, contar uma quantidade de pontos tal que essa quantia seja 
maior que a maior quantidade de linhas potencialemente obstruídas por uma única casa, pois é
mais importante impedir uma vitória que ocupar uma posição numa fila vazia. Neste caso, a 
pontuação escolhida foi 50, apenas por segurança, mas poderia ser menor. Se há uma fila qua-
-se preenchida favoravelmente, a mesma ideia se aplica, para que a máquina priorize vencer 
ao invés de marcar uma outra posição. Por fim, a pontuação não soma nada se uma fila já es-
tiver obstruída, pois posicioná-la neste lugar não traz benefício algum.
  Essa estratégia torna a implementação do EP pobre em questão de estrutura de dados e algo-
-ritmos, pois sua realização é tão simples, do ponto de vista conceitual, que não necessita
de estruturas como árvores, listas ligadas, pilhas etc. Se o objetivo maior não fosse produ-
zir a função mais eficaz, mas abordar os temas tratados no curso, faria diferente; no entan-
-to, meu desejo é mais ganhar o campeonato que brincar com árvores.
Referência: Positional Games by Hefetz, Krivelevich, Stojaković and Szabó */

/* CÓDIGO */
/* O coração do código é a função que escolhe jogadas; sua implementação é bem direta: para
uma certa posição (x,y,z), ela investiga o padrão de cada linha reta que passa por ela, como
as retas paralelas a Ox, Oy e Oz (retas classe 1), as paralelas a planos definidos por ares-
-tas do cubo (retas classe 2) e, finalmente, as diagonais de vértices opostos do cubo (retas
classe 3) — estou denominando de classes para separá-las em categorias.
A verificação usa um processo iterativo simples, que é feito com laços for encadeados; infe-
-lizmente, eles ocorre de maneira exaustivamente repetitiva durante a função principal; tal-
-vez houvesse uma forma menos redundante de se fazer isso, mas tive dificuldade em contornar
esse problema, por isso optei por manter os diversos laços similares.
 */

/* PROTÓTIPO DAS FUNÇÕES */

void print_tab(int ***tab, int n);
void print_cam(int ***tab, int n, int camada);
void insere(int ***tab, int n, int cor, int x, int y);
int disponivel(int ***tab, int n, int x, int y);
int ***cria_tab(int n);
int altura(int ***tab, int n, int x, int y);
void receber_jogadas(int ***tab, int n, int cor);
void ia_vs_ia(int ***tab, int n);
int escolheJogada(int *** tab, int n, int cor, int *lin, int *col);
int fim_de_jogo(int ***tab, int n);
int pontuacao(int ***tab, int n, int cor, int x, int y, int z);
int pontos_linha(int n, int qtd_a, int qtd_b);

int main() {
  int n;
  int ***tab;
  char acao;

  printf("Bem-vindo ao jogo da velha 3D n x n x n com gravidade!\n");
  printf("Escolha o tamanho do tabuleiro:\n");
  scanf(" %d", &n);
  tab = cria_tab(n);
  printf("Tabuleiro criado!\n");
  printf("Quer jogar em qual modo?\n[a] Player vs Maquina\n[b] Maquina vs Maquina kkkkkkk\n");
  scanf(" %c", &acao);
  if(acao == 'a'){
    receber_jogadas(tab, n, 1);
  } else {
    ia_vs_ia(tab, n);
  }
  return 0;
}

/* coloca duas máquina para jogar uma contra a outra kkkkkkkkkkkk */
/* spotniks: colocamos duas ias para jogar, sem que elas soubessem kkkkkk */
void ia_vs_ia(int ***tab, int n){
  int contador = 0;
  int vencedor = 0;
  int cor = 1;
  int x, y;
  int max;
  char acao;

  max = n*n*n;

  print_tab(tab, n);
  printf("pressione qualquer tecla para ver proxima jogada\n");
  while (contador < max) {
    scanf(" %c", &acao);
    if(cor == 1){
      printf("Vez da Máquina A\n");
    } else {
      printf("Vez da Máquina B\n");
    }
    escolheJogada(tab, n, -cor, &x, &y);
    insere(tab, n, -cor, x, y);
    print_tab(tab, n);
    contador++;
    cor = -cor;

    vencedor = fim_de_jogo(tab, n);
    if(vencedor == 1){
      printf("Maquina A ganhou! (ela ganha sempre kkkkkkkkkk)\n");
      break;
    } else if(vencedor == -1){
      printf("Maquina B ganhou!\n");
      break;
    }
  }
  if(vencedor == 0){
    printf("empate\n");
  }
}

/* recebe entradas dos jogadores */
void receber_jogadas(int ***tab, int n, int cor) {
  int x, y, z;
  char acao;
  int vencedor = 0;
  int contador = 0;
  int max;

  x = y = z = 0;
  max = n*n*n;

  print_tab(tab, n);
  printf("Desejas jogar primeiro?\n[a] Sim\n[b] Nao\n");
  scanf(" %c", &acao);

  if(acao == 'a'){
    print_tab(tab, n);
    printf("Para jogar, digite dois inteiro x e y\n");
    scanf(" %d %d", &x, &y);
    z = altura(tab, n, x, y);
    insere(tab, n, cor, x, y);
  } else {
    printf("Se tu ganhar da maquina, 100 kwanzas no pix\nMande email para henrique_eduardo_souza@usp.br\n");
  }
  
  while (contador <= max) {
    /* jogada da máquina */
    escolheJogada(tab, n, -cor, &x, &y);
    insere(tab, n, -cor, x, y);
    print_tab(tab, n);
    contador++;

    /* verificação de vencedor */
    vencedor = fim_de_jogo(tab, n);
    if(vencedor == 1){
      printf("Voce ganhou!\n");
      break;
    } else if(vencedor == -1){
      printf("Voce perdeu.\n");
      break;
    }
    
    /* jogada humana */
    scanf(" %d %d", &x, &y);
    if (!disponivel(tab, n, x, y))
      printf("Posição não disponível\n");
    else{
      z = altura(tab, n, x, y);
      insere(tab, n, cor, x, y);
      print_tab(tab, n);
      contador++;
    }

    /* verificação de vencedor */
    vencedor = fim_de_jogo(tab, n);
    if(vencedor == 1){
      printf("Voce ganhou!");
      break;
    } else if(vencedor == -1){
      printf("Voce perdeu.");
      break;
    }
  if(contador == max && vencedor == 0){
    printf("Empate.\n");
  }
  }
}

/* devolve uma jogada feita pela máquina */
int escolheJogada(int *** tab, int n, int cor, int *lin, int *col){
  int i, j, k;
  int maior_pont = -1;
  int atual_pont = 0;
  int x_otimo = 0;
  int y_otimo = 0;

  printf("Maquina pensando na jogada...\n");
  printf("Mapa tatico:\n");
  for(i = 0; i < n; i++){
    for(j = 0; j < n; j++){
      k = altura(tab, n, i, j);
      atual_pont = pontuacao(tab, n, cor, i, j, k);
      if(atual_pont > maior_pont){
        maior_pont = atual_pont;
        x_otimo = i;
        y_otimo = j;
      }
      printf("%d ", atual_pont);
    }
    printf("\n");
  }
  printf("\n");
  
  *lin = x_otimo;
  *col = y_otimo;

  /* caso não exista nenhuma posição disponível */
  if(maior_pont == -1){
    return -1;
  }

  return 0;
}

/* diz se o jogo está ou não finalizado e retorna o índice do jogador campeão */
int fim_de_jogo(int ***tab, int n){
  int qtd_cor_a;
  int qtd_cor_b;
  int i, j, k;

  /* BUSCA PELAS LINHAS CLASSE 1 */

  /* busca pela linha em paralela ao eixo x */
  for(i = 0; i < n; i++){
    for(j = 0; j < n; j++){
      qtd_cor_a = 0;
      qtd_cor_b = 0;
      for(k = 0; k < n; k++){
        if(tab[k][i][j] == 1){
          qtd_cor_a++;
        } else if(tab[k][j][i] == -1){
          qtd_cor_b++;
        }
        if(qtd_cor_a >= 1 && qtd_cor_b >= 1){
          break;
        }
      }
      if(qtd_cor_a == n){
        return 1;
      } else if(qtd_cor_b == n){
        return -1;
      }
    }
  }

  /* busca pela linha em paralela ao eixo y */
  for(i = 0; i < n; i++){
    for(j = 0; j < n; j++){
      qtd_cor_a = 0;
      qtd_cor_b = 0;
      for(k = 0; k < n; k++){
        if(tab[i][k][j] == 1){
          qtd_cor_a++;
        } else if(tab[i][k][i] == -1){
          qtd_cor_b++;
        }
        if(qtd_cor_a >= 1 && qtd_cor_b >= 1){
          break;
        }
      }
      if(qtd_cor_a == n){
        return 1;
      } else if(qtd_cor_b == n){
        return -1;
      }
    }
  }
  
  /* busca pela linha em paralela ao eixo z */
  for(i = 0; i < n; i++){
    for(j = 0; j < n; j++){
      qtd_cor_a = 0;
      qtd_cor_b = 0;
      for(k = 0; k < n; k++){
        if(tab[i][j][k] == 1){
          qtd_cor_a++;
        } else if(tab[i][j][k] == -1){
          qtd_cor_b++;
        }
        if(qtd_cor_a >= 1 && qtd_cor_b >= 1){
          break;
        }
      }
      if(qtd_cor_a == n){
        return 1;
      } else if(qtd_cor_b == n){
        return -1;
      }
    }
  }

  /* busca pela linha do plano com os pontos A, B, G, e H */
  for(i = 0; i < n; i++){
    qtd_cor_a = 0;
    qtd_cor_b = 0;
    for(j = 0; j < n; j++){
      if(tab[i][n-j-1][j] == 1){
        qtd_cor_a++;
      } else if(tab[i][n-j-1][j] == -1){
        qtd_cor_b++;
      }
      if(qtd_cor_a >= 1 && qtd_cor_b >= 1){
        break;
      }

      if(qtd_cor_a == n){
        return 1;
      } else if(qtd_cor_b == n){
        return -1;
      }
    }
  }

  /* busca pela linha do plano com os pontos C, D, E e F */
  for(i = 0; i < n; i++){
    qtd_cor_a = 0;
    qtd_cor_b = 0;
    for(j = 0; j < n; j++){
      if(tab[i][j][j] == 1){
        qtd_cor_a++;
      } else if(tab[i][j][j] == -1){
        qtd_cor_b++;
      }
      if(qtd_cor_a >= 1 && qtd_cor_b >= 1){
        break;
      }

      if(qtd_cor_a == n){
        return 1;
      } else if(qtd_cor_b == n){
        return -1;
      }
    }
  }

  /* busca pela linha do plano com os pontos A, D, F e G */
  for(i = 0; i < n; i++){
    qtd_cor_a = 0;
    qtd_cor_b = 0;
    for(j = 0; j < n; j++){
      if(tab[j][i][j] == 1){
        qtd_cor_a++;
      } else if(tab[j][i][j] == -1){
        qtd_cor_b++;
      }
      if(qtd_cor_a >= 1 && qtd_cor_b >= 1){
        break;
      }

      if(qtd_cor_a == n){
        return 1;
      } else if(qtd_cor_b == n){
        return -1;
      }
    }
  }

  /* busca pela linha do plano com os pontos B, C, E e H */
  for(i = 0; i < n; i++){
    qtd_cor_a = 0;
    qtd_cor_b = 0;
    for(j = 0; j < n; j++){
      if(tab[n-j-1][i][j] == 1){
        qtd_cor_a++;
      } else if(tab[n-j-1][i][j] == -1){
        qtd_cor_b++;
      }
      if(qtd_cor_a >= 1 && qtd_cor_b >= 1){
        break;
      }

      if(qtd_cor_a == n){
        return 1;
      } else if(qtd_cor_b == n){
        return -1;
      }
    }
  }

  /* busca pela linha do plano com os pontos A, C, G e E */
  for(i = 0; i < n; i++){
    qtd_cor_a = 0;
    qtd_cor_b = 0;
    for(j = 0; j < n; j++){
      if(tab[n-j-1][j][i] == 1){
        qtd_cor_a++;
      } else if(tab[n-j-1][j][i] == -1){
        qtd_cor_b++;
      }
      if(qtd_cor_a >= 1 && qtd_cor_b >= 1){
        break;
      }

      if(qtd_cor_a == n){
        return 1;
      } else if(qtd_cor_b == n){
        return -1;
      }
    }
  }

  /* busca pela linha do plano com os pontos B, D, F e H */
  for(i = 0; i < n; i++){
    qtd_cor_a = 0;
    qtd_cor_b = 0;
    for(j = 0; j < n; j++){
      if(tab[j][j][i] == 1){
        qtd_cor_a++;
      } else if(tab[j][j][i] == -1){
        qtd_cor_b++;
      }
      if(qtd_cor_a >= 1 && qtd_cor_b >= 1){
        break;
      }

      if(qtd_cor_a == n){
        return 1;
      } else if(qtd_cor_b == n){
        return -1;
      }
    }
  }

  return 0;
}

/* para uma posição (x,y,z), retorna a pontuação correspondente */
int pontuacao(int ***tab, int n, int cor, int x, int y, int z){
  int pontos = 0;
  int i, j, k;
  int qtd_cor_a = 0;
  int qtd_cor_b = 0;

  qtd_cor_a = qtd_cor_b = 0;
  i = j = k = 0;

  if(z == n){
    return -1;
  }

  /* BUSCA PELAS LINHAS CLASSE 1 */
  /* busca pela linha em paralela ao eixo x */
  qtd_cor_a = 0;
  qtd_cor_b = 0;
  for(i = 0; i < n; i++){
    /* contagem de cores */
    if(tab[i][y][z] == cor)
      qtd_cor_a++;
    else if(tab[i][y][z] == -cor)
      qtd_cor_b++;

    /* caso de linha obstruída */
    if(qtd_cor_a >= 1 && qtd_cor_b >= 1){
      break;
    }
  }
  /* soma dos pontos */
  pontos += pontos_linha(n, qtd_cor_a, qtd_cor_b);

  /* busca pela linha em paralela ao eixo y */
  qtd_cor_a = 0;
  qtd_cor_b = 0;
  for(i = 0; i < n; i++){
    /* contagem de cores */
    if(tab[x][i][z] == cor)
      qtd_cor_a++;
    else if(tab[x][i][z] == -cor)
      qtd_cor_b++;

    /* caso de linha obstruída */
    if(qtd_cor_a >= 1 && qtd_cor_b >= 1){
      break;
    }
  }
  /* soma dos pontos */
  pontos += pontos_linha(n, qtd_cor_a, qtd_cor_b);

  /* busca pela linha em paralela ao eixo z */
  qtd_cor_a = 0;
  qtd_cor_b = 0;
  for(i = 0; i < n; i++){
    /* contagem de cores */
    if(tab[x][y][i] == cor)
      qtd_cor_a++;
    else if(tab[x][y][i] == -cor)
      qtd_cor_b++;

    /* caso de linha obstruída */
    if(qtd_cor_a >= 1 && qtd_cor_b >= 1){
      break;
    }
  }
  /* soma dos pontos */
  pontos += pontos_linha(n, qtd_cor_a, qtd_cor_b);

  /* BUSCA PELAS LINHAS CLASSE 2 */
  /* busca pela linha do plano com os pontos A, B, G, e H */
  qtd_cor_a = 0;
  qtd_cor_b = 0;
  if(z + y == n - 1){
    for(i = 0; i < n; i++){
      /* contagem de cores */
      if(tab[x][n-i-1][i] == cor)
        qtd_cor_a++;
      else if(tab[x][n-i-1][i] == -cor)
        qtd_cor_b++;

      /* caso de linha obstruída */
      if(qtd_cor_a >= 1 && qtd_cor_b >= 1){
        break;
      }
    }
    /* soma dos pontos */
    pontos += pontos_linha(n, qtd_cor_a, qtd_cor_b);
  }

  /* busca pela linha do plano com os pontos C, D, E, e F */
  qtd_cor_a = 0;
  qtd_cor_b = 0;
  if(z == y){
    for(i = 0; i < n; i++){
      /* contagem de cores */
      if(tab[x][i][i] == cor)
        qtd_cor_a++;
      else if(tab[x][i][i] == -cor)
        qtd_cor_b++;

      /* caso de linha obstruída */
      if(qtd_cor_a >= 1 && qtd_cor_b >= 1){
        break;
      }
    }
    /* soma dos pontos */
    pontos += pontos_linha(n, qtd_cor_a, qtd_cor_b);
  }

  /* busca pela linha do plano com os pontos A, D, F, e G */
  qtd_cor_a = 0;
  qtd_cor_b = 0;
  if(x == z){
    for(i = 0; i < n; i++){
      /* contagem de cores */
      if(tab[i][y][i] == cor)
        qtd_cor_a++;
      else if(tab[x][y][i] == -cor)
        qtd_cor_b++;

      /* caso de linha obstruída */
      if(qtd_cor_a >= 1 && qtd_cor_b >= 1){
        break;
      }
    }
    /* soma dos pontos */
    pontos += pontos_linha(n, qtd_cor_a, qtd_cor_b);
  }

  /* busca pela linha do plano com os pontos B, C, E, e H */
  qtd_cor_a = 0;
  qtd_cor_b = 0;
  if(x + z == n - 1){
    for(i = 0; i < n; i++){
      /* contagem de cores */
      if(tab[n-i-1][y][i] == cor)
        qtd_cor_a++;
      else if(tab[n-i-1][y][i] == -cor)
        qtd_cor_b++;

      /* caso de linha obstruída */
      if(qtd_cor_a >= 1 && qtd_cor_b >= 1){
        break;
      }
    }
    /* soma dos pontos */
    pontos += pontos_linha(n, qtd_cor_a, qtd_cor_b);
  }

  /* busca pela linha do plano com os pontos A, C, G, e E */
  qtd_cor_a = 0;
  qtd_cor_b = 0;
  if(x + y == n - 1){
    for(i = 0; i < n; i++){
      /* contagem de cores */
      if(tab[n-i-1][i][z] == cor)
        qtd_cor_a++;
      else if(tab[n-i-1][i][z] == -cor)
        qtd_cor_b++;

      /* caso de linha obstruída */
      if(qtd_cor_a >= 1 && qtd_cor_b >= 1){
        break;
      }
    }
    /* soma dos pontos */
    pontos += pontos_linha(n, qtd_cor_a, qtd_cor_b);
  }

  /* busca pela linha do plano com os pontos B, D, F, e H */
  qtd_cor_a = 0;
  qtd_cor_b = 0;
  if(x == y){
    for(i = 0; i < n; i++){
      /* contagem de cores */
      if(tab[i][i][z] == cor)
        qtd_cor_a++;
      else if(tab[i][i][z] == -cor)
        qtd_cor_b++;

      /* caso de linha obstruída */
      if(qtd_cor_a >= 1 && qtd_cor_b >= 1){
        break;
      }
    }
    /* soma dos pontos */
    pontos += pontos_linha(n, qtd_cor_a, qtd_cor_b);
  }

  /* BUSCA PELAS LINHAS CLASSE 3 */
  /* busca pela linha diagonal AG */
  qtd_cor_a = 0;
  qtd_cor_b = 0;
  if(x + y == n - 1 && y + z == n - 1){
    for(i = 0; i < 0; i++){
      /* contagem de cores */
      if(tab[i][n-i-1][i] == cor)
        qtd_cor_a++;
      else if(tab[i][n-i-1][i] == -cor)
        qtd_cor_b++;

      /* caso de linha obstruída */
      if(qtd_cor_a >= 1 && qtd_cor_b >= 1)
        break;
    }
  }
  /* soma dos pontos */
    pontos += pontos_linha(n, qtd_cor_a, qtd_cor_b);

  /* busca pela linha diagonal BH */
  qtd_cor_a = 0;
  qtd_cor_b = 0;
  if(x == y && x + z == n - 1){
    for(i = 0; i < 0; i++){
      /* contagem de cores */
      if(tab[n-i-1][n-i-1][i] == cor)
        qtd_cor_a++;
      else if(tab[n-i-1][n-i-1][i] == -cor)
        qtd_cor_b++;

      /* caso de linha obstruída */
      if(qtd_cor_a >= 1 && qtd_cor_b >= 1)
        break;
    }
  }
  /* soma dos pontos */
    pontos += pontos_linha(n, qtd_cor_a, qtd_cor_b);

  /* busca pela linha diagonal CE */
  qtd_cor_a = 0;
  qtd_cor_b = 0;
  if(x + y == n - 1 && x + z == n - 1){
    for(i = 0; i < 0; i++){
      /* contagem de cores */
      if(tab[n-i-1][i][i] == cor)
        qtd_cor_a++;
      else if(tab[n-i-1][i][i] == -cor)
        qtd_cor_b++;

      /* caso de linha obstruída */
      if(qtd_cor_a >= 1 && qtd_cor_b >= 1)
        break;
    }
  }
  /* soma dos pontos */
    pontos += pontos_linha(n, qtd_cor_a, qtd_cor_b);
    
  /* busca pela linha diagonal DF */
  qtd_cor_a = 0;
  qtd_cor_b = 0;
  if(x == y && x == z){
    for(i = 0; i < 0; i++){
      /* contagem de cores */
      if(tab[i][i][i] == cor)
        qtd_cor_a++;
      else if(tab[i][i][i] == -cor)
        qtd_cor_b++;

      /* caso de linha obstruída */
      if(qtd_cor_a >= 1 && qtd_cor_b >= 1)
        break;
    }
  }
  /* soma dos pontos */
    pontos += pontos_linha(n, qtd_cor_a, qtd_cor_b);

  return pontos;
}

/* retorna a pontuação correspondente de uma certa linha */
int pontos_linha(int n, int qtd_a, int qtd_b){
  if(qtd_a >= 1 && qtd_b == 0){
    /* caso de linha apenas da cor desejada */
    if(qtd_a == n - 1)
      return 50;
    return 1;
  } else if(qtd_a == 0 && qtd_b >= 1){
    /* caso de linha apenas da cor do inimigo */
    if(qtd_b == n - 1)
      return 50;
    return 20;
  } else if(qtd_a == 0 && qtd_b == 0){
    /* caso de linha vazia */
    return 1;
  }
  return 0;
}

/* imprime todo o tabuleiro em camadas, por ordem crescente */
void print_tab(int ***tab, int n) {
  int i;
  /* impressão camada a camada */
  for (i = n - 1; i >= 0; i--) {
    print_cam(tab, n, i);
    printf("\n");
  }
}

/* imprime uma única camada do tabuleiro */
void print_cam(int ***tab, int n, int camada) {
  int i, j;

  printf("Camada %d:\n", camada);
  for (i = 0; i < n; i++) {
    /* impressão linha a linha */
    for (j = 0; j < n; j++) {
      /* impressão de cada elemento de cada coluna */
      if(tab[i][j][camada] == 1){
        printf(" 1 ");
      } else if(tab[i][j][camada] == -1){
        printf("-1 ");
      } else {
        printf(" 0 ");
      }
      
    }
    printf("\n");
  }
}

/* insere uma bolinha de uma certa cor n posição (x,y) */
void insere(int ***tab, int n, int cor, int x, int y) {
  int i;
  /* procurando primeira posição vazia, de baixo para cima */
  i = 0;
  while (tab[x][y][i] != 0)
    i++;

  tab[x][y][i] = cor;
}

/* verifica se a posição (x,y) não está cheia*/
int disponivel(int ***tab, int n, int x, int y) {
  /* verifica se a posição (x,y) da última camada está vazia */
  if (tab[x][y][n - 1] != 0)
    return 0;
  return 1;
}

/* retorna um ponteiro 3D correspondente ao tabuleiro */
int ***cria_tab(int n) {
  int i, j, k;
  int ***tab;

  /* alocação dinâmica de memória para o tabuleiro */
  tab = malloc(n * sizeof(int **));
  for (i = 0; i < n; i++) {
    tab[i] = malloc(n * sizeof(int *));
    for (j = 0; j < n; j++)
      tab[i][j] = malloc(n * sizeof(int));
  }

  /* zera todas as entradas por padrão */
  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++)
      for (k = 0; k < n; k++)
        tab[i][j][k] = 0;

  return tab;
}

/* retorna a altura da pilha na posição (x,y) */
int altura(int ***tab, int n, int x, int y){
  int h = 0;
  if(tab[x][y][n-1] != 0){
    return n;
  }
  while(tab[x][y][h] != 0){
    h++;
  }
  return h;
}