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


// Estrutura para grafo
typedef struct Grafo {
    int numVertices;
    NO* listasAdj[MAX_VERTICES];
    int grauEntrada[MAX_VERTICES]; // Grau de entrada de cada vértice
} Grafo;

// Função para criar um novo nó
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
    
    // Inicializar listas de adjacência e graus de entrada
    for (int i = 0; i < vertices; i++) {
        grafo->listasAdj[i] = NULL;
        grafo->grauEntrada[i] = 0;
    }
    
    return grafo;
}

// Função para adicionar uma aresta ao grafo
void adicionarAresta(Grafo* grafo, int origem, int destino) {
    NO* novoNO = criarNO(destino);
    novoNO->proximo = grafo->listasAdj[origem];
    grafo->listasAdj[origem] = novoNO;
    
    grafo->grauEntrada[destino]++;
}

// Função para imprimir o grafo
void imprimirGrafo(Grafo* grafo) {
    for (int i = 0; i < grafo->numVertices; i++) {
        NO* temp = grafo->listasAdj[i];
        printf("Vértice %d:", i);
        while (temp) {
            printf(" -> %d", temp->vertice);
            temp = temp->proximo;
        }
        printf("\n");
    }
}

// Função para realizar a ordenação topológica usando busca em largura
void ordenacaoTopologica(Grafo* grafo) {
    int numVertices = grafo->numVertices;
    int fila[MAX_VERTICES], inicio = 0, final = 0;
    int ordemTopologica[MAX_VERTICES];
    int indice = 0;
    
    for (int i = 0; i < numVertices; i++) {
        if (grafo->grauEntrada[i] == 0) {
            fila[final++] = i; // Adiciona o vértice no final da fila, que tenha grau 0
        }
    }
    
    while (inicio < final) {
        int u = fila[inicio++];//remove o primeiro elemento da fila
        ordemTopologica[indice++] = u; //adiciona o vértice a ordem
        
        NO* temp = grafo->listasAdj[u]; //pega a lista de adjacência ao vértice
        while (temp != NULL) {
            int v = temp->vertice;
            grafo->grauEntrada[v]--;//Diminui o grau de entrada do vértice adjacente a 'u'
            
            if (grafo->grauEntrada[v] == 0) {
                fila[final++] = v;//Adiciona o vértice adjacente ao final da fila
            }
            
            temp = temp->proximo;//Passa para o próximo vértice
        }
    }
    
    if (indice != numVertices) {
        printf("O grafo tem um ciclo e não pode ser ordenado topologicamente.\n");
    } else {
        printf("A ordenação topológica é: ");
        for (int i = 0; i < numVertices; i++) {
            printf("%d ", ordemTopologica[i]);
        }
        printf("\n");
    }
}

int main() {
    int vertices = 8;
    int origem;
    int destino;
    Grafo* grafo = criarGrafo(vertices);
    
    for (int i = 0; i < vertices; i++){
        printf("Qual é a origem da aresta: ");
        scanf("%d", &origem);

        printf("Qual é o destino da aresta: ");
        scanf("%d", &destino);

        adicionarAresta(grafo, origem, destino);
        printf("Vertice: %d\n\n", i);
    }
    //adicionarAresta(grafo, 1, 2);
    //adicionarAresta(grafo, 2, 3);
    //adicionarAresta(grafo, 3, 4);
    //adicionarAresta(grafo, 4, 5);
    //adicionarAresta(grafo, 4, 5);
    //adicionarAresta(grafo, 5, 6);
    //adicionarAresta(grafo, 6, 8);
    //adicionarAresta(grafo, 8, 12);
    imprimirGrafo(grafo);

    ordenacaoTopologica(grafo);
    
    return 0;
}
