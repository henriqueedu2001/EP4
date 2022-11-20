#include <stdio.h>
#include <stdlib.h>

void receber_jogadas(int ***tab, int n, int cor);
void jogar_ia();
void print_tab(int ***tab, int n);
void print_cam(int ***tab, int n, int camada);
int insere(int ***tab, int n, int cor, int x, int y);
int disponivel(int ***tab, int n, int x, int y);
int cheio(int ***tab, int n);
int ***cria_tab(int n);


int main() {
  int ***tab = cria_tab(4);
  print_tab(tab, 4);
  receber_jogadas(tab, 4, 1);
  return 0;
}

void receber_jogadas(int ***tab, int n, int cor) {
  int x, y;

  while (1) {
    print_tab(tab, n);
    scanf(" %d %d", &x, &y);

    /* término do jogo */
    if (x == -1 || y == -1)
      break;
    if (!disponivel(tab, n, x, y))
      printf("Posição não disponível\n");
    else
      insere(tab, n, cor, x, y);
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
int insere(int ***tab, int n, int cor, int x, int y) {
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


int cheio(int ***tab, int n){
  int i, j;
  
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