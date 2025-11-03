#ifndef UTIL_H_   // Verifica se o cabeçalho UTIL_H_ ainda não foi definido
#define UTIL_H_   // Define o cabeçalho UTIL_H_ para evitar inclusões múltiplas

#include <stdio.h>    // Inclui funções de entrada e saída padrão (printf, fopen, etc.)
#include <math.h>     // Inclui funções matemáticas (sqrt, pow, etc.)
#include <limits.h>   // Inclui constantes de limites (INT_MAX, etc.)
#include <unistd.h>   // Inclui funções de sistema POSIX (sleep, usleep, etc.)

// Define o tamanho fixo da grade (matriz do labirinto)
#define LINHAS 32
#define COLUNAS 32

// Estrutura que representa um nó (ou célula) no labirinto
typedef struct {
    int parent_x, parent_y; // Coordenadas do nó pai (para reconstruir o caminho)
    int g;                  // Custo do início até o nó atual
    int h;                  // Heurística (estimativa do custo até o objetivo)
    int f;                  // Soma de g + h (usado no algoritmo A*)
    int is_obstacle;        // Indica se o nó é um obstáculo (1 = sim, 0 = não)
    int visited;            // Indica se o nó já foi visitado (1 = sim, 0 = não)
    int is_path;            // Indica se o nó faz parte do caminho final (1 = sim, 0 = não)
} Node;

// Declarações externas (variáveis globais definidas em outro arquivo .c)
extern Node grid[LINHAS][COLUNAS];  // Matriz de nós representando o labirinto
extern int frame_count;              // Contador de quadros (frames) para exportação de animações

// Protótipos de funções utilitárias relacionadas ao labirinto
int carregar_labirinto_de_arquivo(const char* filename);
// Lê o labirinto de um arquivo e carrega na matriz 'grid'

void exportar_resultado(const char *filename, int start_x, int start_y, int goal_x, int goal_y);
// Exporta o resultado final (ex: caminho encontrado) para um arquivo

void salvar_estado(const char *nome_arquivo, int start_x, int start_y, int goal_x, int goal_y);
// Salva o estado atual da busca (por exemplo, para depuração ou visualização passo a passo)

void salvar_frame(int start_x, int start_y, int goal_x, int goal_y);
// Salva um "frame" (imagem/estado) da execução, possivelmente usado em animações do processo

#endif // UTIL_H_   // Fim da diretiva de inclusão condicional