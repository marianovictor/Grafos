#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

// Função para inicializar o vetor de vértices para o algoritmo de Prim
void inicializaVertices(int *vertices, int n) {
    vertices[0] = 1;  // O primeiro vértice é incluído no MST
    for (int i = 1; i < n; i++) {
        vertices[i] = 0;  // Os outros vértices ainda não estão na árvore
    }
}

// Função que verifica se ainda há vértices disponíveis para serem incluídos no MST
int disponiveis(int *vertices, int n) {
    for (int i = 1; i < n; i++) {
        if (vertices[i] == 0) {
            return 1;  // Se houver algum vértice que ainda não está na árvore, retorna 1
        }
    }
    return 0;  
}

// Implementação do algoritmo de Prim para encontrar a Árvore Geradora Mínima (MST)
void prim(int **grafo, int n) {
    int *vertices = (int *)malloc(n * sizeof(int));  // Array que indica quais vértices estão na MST
    inicializaVertices(vertices, n);  

    // Enquanto houver vértices a serem incluídos no MST
    while (disponiveis(vertices, n)) {
        int menorPeso = INT_MAX;  // Variável para armazenar o menor peso encontrado
        int a = -1, b = -1;  // Reinicializa a e b

        for (int i = 0; i < n; i++) {
            if (vertices[i] == 1) {  // Verifica se o vértice está na MST
                for (int j = 0; j < n; j++) {
                    if (vertices[j] == 0 && grafo[i][j] > 0 && grafo[i][j] < menorPeso) {  // Verifica se há conexão válida com um vértice fora da MST
                        menorPeso = grafo[i][j];
                        a = i;
                        b = j;
                    }
                }
            }
        }

        // Verifica se encontramos uma aresta válida para incluir na MST
        if (b != -1) {
            vertices[b] = 1;  // Inclui o vértice 'b' na MST
            //printf("%d - %d  Peso: %d \n", a, b, grafo[a][b]);  
        } else {
            // Se não conseguimos encontrar uma aresta válida, saímos do loop
            break;
        }
    }
    free(vertices);  
}



// Função para gerar um grafo aleatório no modelo Erdős-Rényi
void gerarGrafo(int **matriz, int n, double p) { 
    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {  // Começa de 'i' para garantir a simetria (grafo não direcionado)
            if (i != j) { 
                double prob = (double)rand() / RAND_MAX;  // Gera um número aleatório entre 0 e 1
                if (prob < p) {  // Se o número for menor que a probabilidade 'p', cria uma aresta
                    int peso = rand() % 10 + 1;  // Aresta com peso entre 1 e 10
                    matriz[i][j] = peso;  
                    matriz[j][i] = peso;  // Simetria
                } else {
                    matriz[i][j] = INT_MAX;  // Sem aresta, representado como "infinito"
                    matriz[j][i] = INT_MAX;  // Simetria
                }
            } else {
                matriz[i][j] = 0;  // A diagonal principal representa o peso das auto-arestas, que é 0
            }
        }
    }
}

// Função para imprimir a matriz de adjacência em um arquivo
void imprimeMatriz(int **matriz, int n) {
    FILE *arquivo = fopen("matrizPrim.txt", "w");  
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
}

// Função para substituir valores INT_MAX por 0, facilitando a visualização
void zeraINTMAX(int **matriz, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matriz[i][j] == INT_MAX) {
                matriz[i][j] = 0;  // Substitui "infinito" por 0
            }
        }
    }
}

int main() {
    srand(time(0));  // Semente para gerar números aleatórios

    int n = 3250;  // Variável que define o número de vértices
    float p = 0.9; // Probabilidade para as arestas (0 - 1)

    int **grafo = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        grafo[i] = (int *)malloc(n * sizeof(int));
    }

    // Captura o tempo inicial
    clock_t start_time = clock();

    gerarGrafo(grafo, n, p);  

    prim(grafo, n);  

    zeraINTMAX(grafo, n);  
    imprimeMatriz(grafo, n);  

    // Captura o tempo final
    clock_t end_time = clock();

    // Calcula o tempo de execução em segundos
    double time_spent = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Tempo de execução: %f segundos\n", time_spent);

    for (int i = 0; i < n; i++) {
        free(grafo[i]);
    }
    free(grafo);

    return 0;
}
