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

void insere13679972(int ***tab, int n, int cor, int x, int y);
int altura13679972(int ***tab, int n, int x, int y);
int escolheJogada13679972(int *** tab, int n, int cor, int *lin, int *col);
int pontuacao13679972(int ***tab, int n, int cor, int x, int y, int z);
int pontos_linha13679972(int n, int qtd_a, int qtd_b);

/* devolve uma jogada feita pela máquina */
int escolheJogada13679972(int *** tab, int n, int cor, int *lin, int *col){
  int i, j, k;
  int maior_pont = 0;
  int atual_pont = 0;
  int x_otimo = 0;
  int y_otimo = 0;

  for(i = 0; i < n; i++){
    for(j = 0; j < n; j++){
      k = altura13679972(tab, n, i, j);
      atual_pont = pontuacao13679972(tab, n, cor, i, j, k);
      if(atual_pont > maior_pont){
        maior_pont = atual_pont;
        x_otimo = i;
        y_otimo = j;
      }
    }
  }

  *lin = x_otimo;
  *col = y_otimo;

  /* caso não exista nenhuma posição disponível */
  if(maior_pont == -1){
    return -1;
  }

  return 0;
}

/* para uma posição (x,y,z), retorna a pontuação correspondente */
int pontuacao13679972(int ***tab, int n, int cor, int x, int y, int z){
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
  pontos += pontos_linha13679972(n, qtd_cor_a, qtd_cor_b);

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
  pontos += pontos_linha13679972(n, qtd_cor_a, qtd_cor_b);

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
  pontos += pontos_linha13679972(n, qtd_cor_a, qtd_cor_b);

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
    pontos += pontos_linha13679972(n, qtd_cor_a, qtd_cor_b);
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
    pontos += pontos_linha13679972(n, qtd_cor_a, qtd_cor_b);
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
    pontos += pontos_linha13679972(n, qtd_cor_a, qtd_cor_b);
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
    pontos += pontos_linha13679972(n, qtd_cor_a, qtd_cor_b);
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
    pontos += pontos_linha13679972(n, qtd_cor_a, qtd_cor_b);
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
    pontos += pontos_linha13679972(n, qtd_cor_a, qtd_cor_b);
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
    pontos += pontos_linha13679972(n, qtd_cor_a, qtd_cor_b);

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
    pontos += pontos_linha13679972(n, qtd_cor_a, qtd_cor_b);

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
    pontos += pontos_linha13679972(n, qtd_cor_a, qtd_cor_b);
    
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
    pontos += pontos_linha13679972(n, qtd_cor_a, qtd_cor_b);

  return pontos;
}

/* retorna a pontuação correspondente de uma certa linha */
int pontos_linha13679972(int n, int qtd_a, int qtd_b){
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

/* retorna a altura da pilha na posição (x,y) */
int altura13679972(int ***tab, int n, int x, int y){
  int h = 0;
  if(tab[x][y][n-1] != 0){
    return n;
  }
  while(tab[x][y][h] != 0){
    h++;
  }
  return h;
}