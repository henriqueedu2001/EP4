#include <stdio.h>
#include <stdlib.h>

void receber_jogadas(int ***tab, int n, int cor);
void jogar_ia();
void print_tab(int ***tab, int n);
void print_cam(int ***tab, int n, int camada);
void insere(int ***tab, int n, int cor, int x, int y);
int disponivel(int ***tab, int n, int x, int y);
int ***cria_tab(int n);
int altura(int ***tab, int n, int x, int y);

int pontuacao(int ***tab, int n, int cor, int x, int y, int z);
int pontos_linha(int qtd_a, int qtd_b);

int main() {
  int ***tab = cria_tab(3);
  receber_jogadas(tab, 3, 1);
  return 0;
}

/* para uma posição (x,y,z), retorna a pontuação correspondente */
int pontuacao(int ***tab, int n, int cor, int x, int y, int z){
  int pontos = 0;
  int i, j, k;
  int qtd_cor_a = 0;
  int qtd_cor_b = 0;

  printf("calculando (%d,%d,%d):\n", x, y, z);

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
  printf("qtd_a = %d; qtd_b = %d\n", qtd_cor_a, qtd_cor_b);
  /* contagem dos pontos */
  pontos += pontos_linha(qtd_cor_a, qtd_cor_b);

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
  printf("qtd_a = %d; qtd_b = %d\n", qtd_cor_a, qtd_cor_b);
  /* contagem dos pontos */
  pontos += pontos_linha(qtd_cor_a, qtd_cor_b);

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
  printf("qtd_a = %d; qtd_b = %d\n", qtd_cor_a, qtd_cor_b);
  /* contagem dos pontos */
  pontos += pontos_linha(qtd_cor_a, qtd_cor_b);
  
  return pontos;
}

int pontos_linha(int qtd_a, int qtd_b){
  if(qtd_a >= 1 && qtd_b == 0){
    /* caso de linha apenas da cor desejada */
    return 1;
  } else if(qtd_a == 0 && qtd_b >= 1){
    /* caso de linha apenas da cor do inimigo */
    return 2;
  } else if(qtd_a == 0 && qtd_b == 0){
    /* caso de linha vazia */
    return 1;
  }
  return 0;
}

void receber_jogadas(int ***tab, int n, int cor) {
  int x, y, z;
  int contador = cor;

  print_tab(tab, n);
  while (1) {
    
    scanf(" %d %d", &x, &y);

    /* término do jogo */
    if (x == -1 || y == -1)
      break;
    if (!disponivel(tab, n, x, y))
      printf("Posição não disponível\n");
    else{
      z = altura(tab, n, x, y);
      printf("pontuacao: %d\n", pontuacao(tab, n, contador, x, y, z));
      insere(tab, n, contador, x, y);
      
    }
    print_tab(tab, n);
    contador = -contador;
  }
}

void jogar_ia(){
  
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