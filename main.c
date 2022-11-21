#include <stdio.h>
#include <stdlib.h>

int ACABOU = 0;

void print_tab(int ***tab, int n);
void print_cam(int ***tab, int n, int camada);
void insere(int ***tab, int n, int cor, int x, int y);
int disponivel(int ***tab, int n, int x, int y);
int ***cria_tab(int n);
int altura(int ***tab, int n, int x, int y);

void receber_jogadas(int ***tab, int n, int cor);
int escolheJogada(int *** tab, int n, int cor, int *lin, int *col);
int fim_de_jogo(int ***tab, int n);
int pontuacao(int ***tab, int n, int cor, int x, int y, int z);
int pontos_linha(int n, int qtd_a, int qtd_b);

int main() {
  int n;
  int ***tab;

  printf("Bem-vindo ao jogo da velha 3D n x n x n com gravidade!\n");
  printf("Escolha o tamanho do tabuleiro:\n");
  scanf(" %d", &n);
  tab = cria_tab(n);
  printf("Tabuleiro criado!\n");

  receber_jogadas(tab, n, 1);
  return 0;
}

/* recebe entradas dos jogadores */
void receber_jogadas(int ***tab, int n, int cor) {
  int x, y, z;
  char acao;
  int vencedor;

  printf("Desejas jogar primeiro?\n[a] Sim\n[b] Nao\n");
  scanf(" %c", &acao);

  if(acao == 'a'){
    print_tab(tab, n);
    printf("Para jogar, digite dois inteiro x e y\n");
    scanf(" %d %d", &x, &y);
    z = altura(tab, n, x, y);
    insere(tab, n, cor, x, y);
  }
  print_tab(tab, n);
  while (1) {
    /* jogada da máquina */
    escolheJogada(tab, n, -cor, &x, &y);
    insere(tab, n, -cor, x, y);
    print_tab(tab, n);

    vencedor = fim_de_jogo(tab, n);
    if(vencedor == 1){
      printf("Voce ganhou!");
      break;
    } else if(vencedor == -1){
      printf("Voce perdeu.");
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
    }

    vencedor = fim_de_jogo(tab, n);
    if(vencedor == 1){
      printf("Voce ganhou!");
      break;
    } else if(vencedor == -1){
      printf("Voce perdeu.");
      break;
    }
  }
}

/* devolve uma jogada feita pela máquina */
int escolheJogada(int *** tab, int n, int cor, int *lin, int *col){
  int i, j, k;
  int maior_pont = 0;
  int atual_pont = 0;
  int x_otimo = 0;
  int y_otimo = 0;

  printf("Maquina pensando na jogada...\n");

  for(i = 0; i < n; i++){
    for(j = 0; j < n; j++){
      k = altura(tab, n, i, j);
      atual_pont = pontuacao(tab, n, cor, i, j, k);
      if(atual_pont > maior_pont){
        maior_pont = atual_pont;
        x_otimo = i;
        y_otimo = j;
      }
      printf("%d ", pontuacao(tab, n, cor, i, j, k));
    }
    printf("\n");
  }

  *lin = x_otimo;
  *col = y_otimo;

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

  return 0;
}

/* para uma posição (x,y,z), retorna a pontuação correspondente */
int pontuacao(int ***tab, int n, int cor, int x, int y, int z){
  int pontos = 0;
  int i, j, k;
  int qtd_cor_a = 0;
  int qtd_cor_b = 0;

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
  if(qtd_a == n || qtd_b == n){
    ACABOU = 1;
    return 0;
  }
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
      printf("%d ", tab[i][j][camada]);
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
  while(tab[x][y][h] != 0){
    h++;
  }
  return h;
}