/*
GRUPO:
Eduardo Costa Araújo
Gabriel Palmeira Luchi
João Pedro Bastasini
Thiago Henrique Uliame
Victor Mariano Rocha
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_VERTICES 100

typedef struct NO {
    int vertice;
    struct NO* proximo;
} NO;

typedef struct Grafo {
    int numVertices;
    NO* listasAdj[MAX_VERTICES];
    int grauEntrada[MAX_VERTICES];
} Grafo;

// Função para criar um novo nó de adjacência
NO* criarNO(int v) {
    NO* novoNO = (NO*)malloc(sizeof(NO));
    novoNO->vertice = v;
    novoNO->proximo = NULL;
    return novoNO;
}

// Função para inicializar o grafo
Grafo* criarGrafo(int vertices) {
    Grafo* grafo = (Grafo*)malloc(sizeof(Grafo));
    grafo->numVertices = vertices;

    for (int i = 0; i < vertices; i++) {
        grafo->listasAdj[i] = NULL;
        grafo->grauEntrada[i] = 0;
    }

    return grafo;
}

// Função para adicionar uma aresta ao grafo não-orientado
void adicionarAresta(Grafo* grafo, int origem, int destino) {
    NO* novoNO = criarNO(destino);
    novoNO->proximo = grafo->listasAdj[origem];
    grafo->listasAdj[origem] = novoNO;
    grafo->grauEntrada[origem]++;

    novoNO = criarNO(origem);
    novoNO->proximo = grafo->listasAdj[destino];
    grafo->listasAdj[destino] = novoNO;
    grafo->grauEntrada[destino]++;
}

// Função auxiliar para verificar se todos os vértices têm grau par
bool todosGrausPares(Grafo* grafo) {
    for (int i = 0; i < grafo->numVertices; i++) {
        if (grafo->grauEntrada[i] % 2 != 0) {
            return false;
        }
    }
    return true;
}

// Função auxiliar para verificar se o grafo é conexo usando busca em profundidade
void dfs(Grafo* grafo, int v, bool visitados[]) {
    visitados[v] = true;
    NO* adj = grafo->listasAdj[v];
    while (adj != NULL) {
        if (!visitados[adj->vertice]) {
            dfs(grafo, adj->vertice, visitados);
        }
        adj = adj->proximo;
    }
}

bool grafoConexo(Grafo* grafo) {
    bool visitados[MAX_VERTICES] = {false};
    int i;
    for (i = 0; i < grafo->numVertices; i++) {
        if (grafo->listasAdj[i] != NULL) {
            break;
        }
    }
    if (i == grafo->numVertices) return true; // Grafo vazio é considerado conexo

    dfs(grafo, i, visitados);

    for (i = 0; i < grafo->numVertices; i++) {
        if (grafo->listasAdj[i] != NULL && !visitados[i]) {
            return false;
        }
    }
    return true;
}

// Função para verificar as condições de ciclo Euleriano
bool verificarCicloEuleriano(Grafo* grafo) {
    return grafoConexo(grafo) && todosGrausPares(grafo);
}

// Função para eulerizar o grafo (adicionar arestas para tornar os graus pares)
void eulerizarGrafo(Grafo* grafo) {
    for (int i = 0; i < grafo->numVertices; i++) {
        if (grafo->grauEntrada[i] % 2 != 0) {
            for (int j = i + 1; j < grafo->numVertices; j++) {
                if (grafo->grauEntrada[j] % 2 != 0) {
                    adicionarAresta(grafo, i, j);
                    break;
                }
            }
        }
    }
}

// Algoritmo de Hierholzer para encontrar ciclo euleriano
void encontrarCicloEuleriano(Grafo* grafo, int inicio) {
    int pilha[MAX_VERTICES], topo = 0;
    int ciclo[MAX_VERTICES], tamCiclo = 0;
    pilha[topo++] = inicio;

    while (topo > 0) {
        int v = pilha[topo - 1];
        if (grafo->listasAdj[v] != NULL) {
            NO* temp = grafo->listasAdj[v];
            int u = temp->vertice;
            grafo->listasAdj[v] = temp->proximo;
            free(temp);
            
            NO* prev = NULL;
            NO* atual = grafo->listasAdj[u];
            while (atual != NULL && atual->vertice != v) {
                prev = atual;
                atual = atual->proximo;
            }
            if (prev == NULL) {
                grafo->listasAdj[u] = atual->proximo;
            } else {
                prev->proximo = atual->proximo;
            }
            free(atual);
            pilha[topo++] = u;
        } else {
            ciclo[tamCiclo++] = v;
            topo--;
        }
    }

    printf("Ciclo Euleriano encontrado:\n");
    for (int i = tamCiclo - 1; i >= 0; i--) {
        printf("%d ", ciclo[i]);
    }
    printf("\n");
}

// Função principal para executar o teste e encontrar o ciclo Euleriano
void verificaCiclo(Grafo* grafo) {
    if (verificarCicloEuleriano(grafo)) {
        printf("O grafo já possui um ciclo euleriano.\n");
    } else {
        printf("O grafo não possui ciclo euleriano. Realizando eulerização...\n");
        eulerizarGrafo(grafo);
    }
    encontrarCicloEuleriano(grafo, 0);
}

int main() {
    Grafo* grafo = criarGrafo(5); 
    adicionarAresta(grafo, 0, 1);
    adicionarAresta(grafo, 1, 2);
    adicionarAresta(grafo, 2, 3);
    adicionarAresta(grafo, 3, 0);
    adicionarAresta(grafo, 3, 4);
    adicionarAresta(grafo, 4, 0);

    verificaCiclo(grafo);

    return 0;
}
