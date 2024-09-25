#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct No {
  int vertice;
  struct No *proximo;
} No;

typedef struct ListaAdj {
  No *cabeca;
} ListaAdj;

No *criarNo(int vertice) {
  No *novoNo = (No *)malloc(sizeof(No));
  novoNo->vertice = vertice;
  novoNo->proximo = NULL;
  return novoNo;
}

void adicionarAresta(ListaAdj *lista, int vertice) {
  No *novoNo = criarNo(vertice);
  novoNo->proximo = lista->cabeca;
  lista->cabeca = novoNo;
}

void imprimirListaAdjacencia(ListaAdj *lista, int tam) {
  printf("Lista de Adjacência:\n");
  for (int i = 0; i < tam; i++) {
    No *temp = lista[i].cabeca;
    printf("Vertice %d:", i);
    while (temp) {
      printf(" -> %d", temp->vertice);
      temp = temp->proximo;
    }
    printf("\n");
  }
}

// Função de busca em largura (BFS)
void bfs(ListaAdj *listaAdj, int inicio, int destino, int tam) {
  bool *visitado = (bool *)malloc(tam * sizeof(bool));
  int *predecessor = (int *)malloc(tam * sizeof(int));
  int *fila = (int *)malloc(tam * sizeof(int));
  int frente = 0, tras = 0;

  for (int i = 0; i < tam; i++) {
    visitado[i] = false;
    predecessor[i] = -1;
  }

  visitado[inicio] = true;
  fila[tras++] = inicio;

  while (frente != tras) {
    int atual = fila[frente++];
    No *temp = listaAdj[atual].cabeca;

    while (temp) {
      int adj = temp->vertice;
      if (!visitado[adj]) {
        visitado[adj] = true;
        predecessor[adj] = atual;
        fila[tras++] = adj;

        if (adj == destino) {
          break;
        }
      }
      temp = temp->proximo;
    }
  }

  // Imprime o caminho entre os vértices s e t
  if (!visitado[destino]) {
    printf("Não há caminho entre os vértices %d e %d\n", inicio, destino);
  } else {
    printf("Caminho entre os vértices %d e %d: ", inicio, destino);
    int caminho[tam];
    int cont = 0;
    for (int v = destino; v != -1; v = predecessor[v]) {
      caminho[cont++] = v;
    }
    for (int i = cont - 1; i >= 0; i--) {
      printf("%d ", caminho[i]);
    }
    printf("\n");
  }

  free(visitado);
  free(predecessor);
  free(fila);
}

// Função de busca em profundidade (DFS) usando pilha
void dfsIterativa(ListaAdj *listaAdj, int inicio, int tam) {
  bool *visitado = (bool *)malloc(tam * sizeof(bool));
  int *pilha = (int *)malloc(tam * sizeof(int));
  int topo = -1;

  for (int i = 0; i < tam; i++) {
    visitado[i] = false;
  }

  pilha[++topo] = inicio;

  while (topo >= 0) {
    int atual = pilha[topo--];

    if (!visitado[atual]) {
      visitado[atual] = true;
      printf("%d ", atual);
    }

    No *temp = listaAdj[atual].cabeca;
    while (temp) {
      int adj = temp->vertice;
      if (!visitado[adj]) {
        pilha[++topo] = adj;
      }
      temp = temp->proximo;
    }
  }

  free(visitado);
  free(pilha);
}

int main() {
  FILE *arquivo;
  int tam, i, j;

  arquivo = fopen("pcv4.txt", "r");
  if (arquivo == NULL) {
    printf("Erro ao abrir o arquivo.\n");
    return 1;
  }

  fscanf(arquivo, "%d", &tam);

  int matriz[tam][tam];

  for (i = 0; i < tam; i++) {
    for (j = 0; j <= i; j++) {
      fscanf(arquivo, "%d", &matriz[i][j]);
      matriz[j][i] = matriz[i][j];
    }
  }
  fclose(arquivo);

  ListaAdj *listaAdj = (ListaAdj *)malloc(tam * sizeof(ListaAdj));

  for (i = 0; i < tam; i++) {
    listaAdj[i].cabeca = NULL;
  }

  // Preenche a lista de adjacência com base na matriz
  for (i = 0; i < tam; i++) {
    for (j = 0; j < tam; j++) {
      if (matriz[i][j] != 0) {
        adicionarAresta(&listaAdj[i], j);
      }
    }
  }

  printf("Matriz (%dx%d):\n", tam, tam);
  for (i = 0; i < tam; i++) {
    for (j = 0; j < tam; j++) {
      printf("%d ", matriz[i][j]);
    }
    printf("\n\n");
  }
  imprimirListaAdjacencia(listaAdj, tam);

  int inicio, destino;
  printf("\nDigite os vértices de origem e destino para a BFS");
  scanf("%d %d", &inicio, &destino);
  bfs(listaAdj, inicio, destino, tam);

  printf("\nDFS iterativa iniciando do vértice 0:\n");
  dfsIterativa(listaAdj, 0, tam);

  // Libera a memória alocada para a lista de adjacência
  for (i = 0; i < tam; i++) {
    No *temp = listaAdj[i].cabeca;
    while (temp) {
      No *prox = temp->proximo;
      free(temp);
      temp = prox;
    }
  }
  free(listaAdj);

  return 0;
}
