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
#include <limits.h>
#include <time.h>

// Função para inicializar a matriz de adjacência com zeros
void inicializaMatriz(int **matriz, int n) {
    for (int i = 0; i < n; i++) { 
        for (int j = 0; j < n; j++) { 
            matriz[i][j] = 0; 
        }
    }
}

// Função para preencher a matriz de adjacência de acordo com a probabilidade p
void gerarGrafo(int **matriz, int n, double p, int orientado) {
    for (int i = 0; i < n; i++) { 
        for (int j = 0; j < n; j++) { 
            double prob = (double)rand() / RAND_MAX; // A divisão pelo RAND_MAX (stdlib) faz com que seja entre 0 e 1
            if (prob < p) { // Condição de Erdos-Rainy que o professor passou
                matriz[i][j] = 1; 
                if (orientado == 0) { 
                    matriz[j][i] = 1; // Se o grafo não é orientado, adiciona a aresta na direção oposta
                }
            }
        }
    }
}

// Função para imprimir a matriz no arquivo
void imprimeMatriz(int **matriz, int n) {
    FILE *arquivo = fopen("matrizGrafo.txt", "w"); 
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

// Função para calcular distâncias usando o algoritmo de Dijkstra
void dijkstra(int **matriz, int n, int comeco, int *distancias) {
    int visitado[n]; 
    int min_distancia[n]; 
    
    for (int i = 0; i < n; i++) {
        min_distancia[i] = INT_MAX; // Inicializa a distância mínima como infinito
        visitado[i] = 0; // Marca todos os nós como não visitados
        distancias[i] = INT_MAX; // Inicializa as distâncias como infinito
    }
    min_distancia[comeco] = 0; 
    distancias[comeco] = 0;
    
    for (int count = 0; count < n - 1; count++) { // Loop para encontrar o menor caminho
        int u = -1; // Nó com a menor distância que ainda não foi visitado
        for (int i = 0; i < n; i++) {
            if (visitado[i] == 0) { 
                if (u == -1 || min_distancia[i] < min_distancia[u]) {
                    u = i; // Atualiza o nó u para o nó com a menor distância
                }
            }
        }
        if (min_distancia[u] == INT_MAX) { // Se a menor distância for infinito, encerra o loop
            break;
        }
        visitado[u] = 1; 
        
        for (int v = 0; v < n; v++) { // Atualiza as distâncias para os vizinhos do nó u
            if (matriz[u][v] == 1 && visitado[v] == 0) { // Se há aresta e o nó v não foi visitado
                if (min_distancia[u] + 1 < min_distancia[v]) {
                    min_distancia[v] = min_distancia[u] + 1; 
                    distancias[v] = min_distancia[v]; 
                }
            }
        }
    }
}

// Função para calcular todas as distâncias usando o algoritmo de Floyd-Warshall
void floyd_warshall(int **matriz, int n, int **distancias) {
    for (int i = 0; i < n; i++) { 
        for (int j = 0; j < n; j++) {
            if (i == j) {
                distancias[i][j] = 0; 
            } else {
                if (matriz[i][j] == 1) {
                    distancias[i][j] = 1; // Distância entre nós conectados diretamente é 1
                } else {
                    distancias[i][j] = INT_MAX; // Se não estão conectados, distância é infinito
                }
            }
        }
    }
    
    for (int k = 0; k < n; k++) { // Itera sobre todos os nós como intermediários
        for (int i = 0; i < n; i++) { // Itera sobre todos os nós de origem
            for (int j = 0; j < n; j++) { // Itera sobre todos os nós de destino
                if (distancias[i][k] != INT_MAX && distancias[k][j] != INT_MAX) { // Verifica se os caminhos são válidos
                    int nova_distancia = distancias[i][k] + distancias[k][j]; 
                    if (nova_distancia < distancias[i][j]) {
                        distancias[i][j] = nova_distancia; 
                    }
                }
            }
        }
    }
}

// Função para analisar o grafo e calcular distâncias máximas e médias
void analisarGrafo(int **matriz, int n) {
    int **distancias = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        distancias[i] = (int *)malloc(n * sizeof(int)); 
    }
    
    floyd_warshall(matriz, n, distancias); // Calcula a matriz de distâncias
    
    int max_distancia = 0; 
    int total_distancia = 0; 
    int count = 0; // Conta o número de distâncias calculadas
    
    for (int i = 0; i < n; i++) { 
        for (int j = 0; j < n; j++) {
            if (distancias[i][j] != INT_MAX) { // Verifica se a distância é finita
                if (distancias[i][j] > max_distancia) { 
                    max_distancia = distancias[i][j]; 
                }
                total_distancia += distancias[i][j]; 
                count++; 
            }
        }
    }
    
    float media_distancia; 
    if (count > 0) {
        media_distancia = (float)total_distancia / count; // Calcula a distância média
    } else {
        media_distancia = 0; // Se não há distâncias válidas, a média é 0
    }
    
    printf("Distância Máxima: %d\n", max_distancia); 
    printf("Distância Média: %.2f\n", media_distancia); 
    
    for (int i = 0; i < n; i++) {
        free(distancias[i]); 
    }
    free(distancias); 
}

int main() {
    int numeroVertices; // Número de vértices (ou nós)
    double p; // Probabilidade de existência de uma aresta
    int orientado; // 1 para orientado, 0 para não-orientado
    int comeco; // Nó inicial para o algoritmo de Dijkstra

    srand(time(NULL));

    printf("Digite o número de vértices: ");
    scanf("%d", &numeroVertices);
    printf("Digite a probabilidade de existência de uma aresta (entre 0 e 1): ");
    scanf("%lf", &p);
    printf("O grafo é orientado? (1 para sim, 0 para não): ");
    scanf("%d", &orientado);

    int **matriz = (int **)malloc(numeroVertices * sizeof(int *));
    for (int i = 0; i < numeroVertices; i++) {
        matriz[i] = (int *)malloc(numeroVertices * sizeof(int));
    }

    inicializaMatriz(matriz, numeroVertices);
    gerarGrafo(matriz, numeroVertices, p, orientado);
    imprimeMatriz(matriz, numeroVertices);

    printf("Digite o nó inicial para o algoritmo de Dijkstra: ");
    scanf("%d", &comeco);

    int *distanciasDijkstra = (int *)malloc(numeroVertices * sizeof(int));

    dijkstra(matriz, numeroVertices, comeco, distanciasDijkstra);

    printf("\nDistâncias a partir do nó %d usando Dijkstra:\n", comeco);
    for (int i = 0; i < numeroVertices; i++) {
        if (distanciasDijkstra[i] == INT_MAX) {
            printf("Distância para o nó %d: Infinito\n", i);
        } else {
            printf("Distância para o nó %d: %d\n", i, distanciasDijkstra[i]);
        }
    }

    printf("\nAnálise do Grafo:\n");
    analisarGrafo(matriz, numeroVertices);

    for (int i = 0; i < numeroVertices; i++) {
        free(matriz[i]);
    }
    free(matriz);
    free(distanciasDijkstra);

    return 0;
}