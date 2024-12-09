#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Estrutura para representar uma aresta de um grafo
struct Aresta {
    int origem, destino;
    int peso;
};

// Estrutura para representar um grafo
struct Grafo {
    int n, E; // n: número de vértices, E: número de arestas
    struct Aresta* arestas; // array de arestas
};

// Estrutura para representar um subconjunto para o algoritmo de união-find
struct Subconjunto {
    int pai;
    int rank;
};

// Função para criar um novo grafo com n vértices e E arestas
struct Grafo* criarGrafo(int n, int E) {
    struct Grafo* grafo = (struct Grafo*) malloc(sizeof(struct Grafo));
    grafo->n = n;
    grafo->E = E;
    grafo->arestas = (struct Aresta*) malloc(E * sizeof(struct Aresta));
    return grafo;
}

// Função para encontrar o conjunto de um elemento i (usando compressão de caminho)
int encontrar(struct Subconjunto subconjuntos[], int i) {
    if (subconjuntos[i].pai != i)
        subconjuntos[i].pai = encontrar(subconjuntos, subconjuntos[i].pai);
    return subconjuntos[i].pai;
}

// Função para unir dois conjuntos x e y (usando união por rank)
void unir(struct Subconjunto subconjuntos[], int x, int y) {
    int raizX = encontrar(subconjuntos, x);
    int raizY = encontrar(subconjuntos, y);

    // União por rank
    if (subconjuntos[raizX].rank < subconjuntos[raizY].rank)
        subconjuntos[raizX].pai = raizY;
    else if (subconjuntos[raizX].rank > subconjuntos[raizY].rank)
        subconjuntos[raizY].pai = raizX;
    else {
        subconjuntos[raizY].pai = raizX;
        subconjuntos[raizX].rank++;
    }
}

// Função para comparar duas arestas com base em seus pesos (usada para ordenação)
int compararArestas(const void* a, const void* b) {
    struct Aresta* a1 = (struct Aresta*)a;
    struct Aresta* a2 = (struct Aresta*)b;
    return a1->peso > a2->peso;
}

// Função para gerar grafo aleatório no modelo de Erdős–Rényi
struct Grafo* gerarGrafoER(int n, float p) {
    int E = 0;
    struct Grafo* grafo = criarGrafo(n, n*(n-1)/2); // Número máximo de arestas

    for (int u = 0; u < n; u++) {
        for (int v = u + 1; v < n; v++) {
            float probabilidade = (float)rand() / RAND_MAX;
            if (probabilidade < p) {
                // Criar uma aresta com peso aleatório
                grafo->arestas[E].origem = u;
                grafo->arestas[E].destino = v;
                grafo->arestas[E].peso = rand() % 1000 + 1; // Peso entre 0 e 1
                E++;
            }
        }
    }
    
    // Ajustar o número de arestas
    grafo->E = E;
    grafo->arestas = (struct Aresta*) realloc(grafo->arestas, E * sizeof(struct Aresta));
    return grafo;
}

// Função para imprimir a matriz de adjacência em um arquivo .txt
void imprimeMatriz(struct Grafo* grafo) {
    int n = grafo->n;
    int** matriz = (int**) malloc(n * sizeof(int*));

    for (int i = 0; i < n; i++) {
        matriz[i] = (int*) malloc(n * sizeof(int));
        for (int j = 0; j < n; j++) {
            matriz[i][j] = 0;
        }
    }

    // Preencher a matriz com as arestas
    for (int i = 0; i < grafo->E; i++) {
        int u = grafo->arestas[i].origem;
        int v = grafo->arestas[i].destino;
        int peso = grafo->arestas[i].peso;
        matriz[u][v] = peso;
        matriz[v][u] = peso; // Simetria, pois o grafo é não direcionado
    }

    // Imprimir a matriz de adjacência em um arquivo .txt
    FILE* arquivo = fopen("matrizKruskal.txt", "w");
    if (arquivo == NULL) {
        perror("Não foi possível abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fprintf(arquivo, "%d ", matriz[i][j]);
        }
        fprintf(arquivo, "\n");
    }

    fclose(arquivo);

    // Liberar memória alocada para a matriz
    for (int i = 0; i < n; i++) {
        free(matriz[i]);
    }
    free(matriz);
}

// Função principal que executa o Algoritmo de Kruskal para encontrar a Árvore Geradora Mínima (AGM)
void kruskal(struct Grafo* grafo) {
    int n = grafo->n;
    struct Aresta resultado[n]; // Array para armazenar o resultado final
    int e = 0; // Índice usado para resultado[]
    int i = 0; // Índice usado para as arestas ordenadas

    // Passo 1: Ordenar todas as arestas em ordem crescente de peso
    qsort(grafo->arestas, grafo->E, sizeof(grafo->arestas[0]), compararArestas);

    // Alocar memória para criar n subconjuntos
    struct Subconjunto* subconjuntos = (struct Subconjunto*) malloc(n * sizeof(struct Subconjunto));

    // Criar subconjuntos com um único elemento
    for (int v = 0; v < n; ++v) {
        subconjuntos[v].pai = v;
        subconjuntos[v].rank = 0;
    }

    // Número de arestas na AGM será sempre n-1
    while (e < n - 1 && i < grafo->E) {
        // Passo 2: Escolher a menor aresta. Selecione a próxima
        struct Aresta proximaAresta = grafo->arestas[i++];

        int x = encontrar(subconjuntos, proximaAresta.origem);
        int y = encontrar(subconjuntos, proximaAresta.destino);

        // Se incluir essa aresta não formar um ciclo, incluir no resultado
        if (x != y) {
            resultado[e++] = proximaAresta;
            unir(subconjuntos, x, y);
        }
    }

    // Exibir o resultado
    /*printf("As arestas na Árvore Geradora Mínima são:\n");
    for (i = 0; i < e; ++i)
        printf("%d -- %d == %d \n", resultado[i].origem, resultado[i].destino, resultado[i].peso);
    */
    free(subconjuntos); // Liberar a memória alocada
}


int main() {
    srand(time(0)); // Semente para gerar números aleatórios

    int n = 3250; // Número de vértices
    float p = 0.9; // Probabilidade de uma aresta existir

    // Captura o tempo inicial
    clock_t start_time = clock();

    // Gerar grafo aleatório no modelo de Erdős–Rényi
    struct Grafo* grafo = gerarGrafoER(n, p);

    kruskal(grafo);


    imprimeMatriz(grafo);

    // Captura o tempo final
    clock_t end_time = clock();

    // Calcula o tempo de execução em segundos
    double time_spent = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Tempo de execução: %f segundos\n", time_spent);


    free(grafo->arestas);
    free(grafo);

    return 0;
}