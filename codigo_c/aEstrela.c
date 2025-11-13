#include "util.h"  // Inclui o arquivo de cabeçalho "util.h", que contém definições, estruturas e funções auxiliares necessárias

// ================================
// Declarações globais
// ================================

// Grade (grid) global representando o mapa do ambiente de busca
Node grid[LINHAS][COLUNAS];

// Contador global de frames (usado para registrar ou animar as etapas da execução)
int frame_count = 0;

// ================================
// Funções auxiliares
// ================================

/**
 * @brief Calcula a distância heurística entre dois pontos utilizando a distância de Manhattan.
 *
 * A heurística é usada pelo algoritmo A* para estimar o custo restante até o objetivo.
 *
 * @param x1 Coordenada X do primeiro ponto
 * @param y1 Coordenada Y do primeiro ponto
 * @param x2 Coordenada X do segundo ponto (objetivo)
 * @param y2 Coordenada Y do segundo ponto (objetivo)
 * @return Distância de Manhattan entre os dois pontos
 */
int heuristic(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

/**
 * @brief Marca o caminho encontrado pelo algoritmo A* na grade.
 *
 * Percorre os nós a partir do objetivo até o início, seguindo os ponteiros para os pais,
 * e marca cada nó como pertencente ao caminho final.
 *
 * @param start_x Coordenada X do ponto inicial
 * @param start_y Coordenada Y do ponto inicial
 * @param goal_x Coordenada X do objetivo
 * @param goal_y Coordenada Y do objetivo
 */
void mark_path_on_grid(int start_x, int start_y, int goal_x, int goal_y) {
    int current_x = goal_x;
    int current_y = goal_y;

    // Percorre do destino até o ponto de partida, reconstruindo o caminho
    while (current_x != start_x || current_y != start_y) {
        grid[current_x][current_y].is_path = 1;  // Marca o nó atual como parte do caminho
        int temp_x = grid[current_x][current_y].parent_x;  // Guarda o pai em X
        int temp_y = grid[current_x][current_y].parent_y;  // Guarda o pai em Y
        current_x = temp_x;  // Atualiza para o próximo nó (pai)
        current_y = temp_y;

        salvar_frame(start_x, start_y, goal_x, goal_y);  // Salva o frame para visualização da reconstrução do caminho
    }

    // Marca também o nó inicial como parte do caminho
    grid[start_x][start_y].is_path = 1;
    salvar_estado("saidas/grid.txt", start_x, start_y, goal_x, goal_y);
}

// ================================
// Função principal do algoritmo A*
// ================================

/**
 * @brief Implementa o algoritmo de busca A* (A-star).
 *
 * O A* combina o custo real do caminho percorrido (g) com a estimativa heurística (h)
 * para encontrar o caminho ótimo do ponto inicial ao objetivo.
 *
 * @param start_x Coordenada X do ponto inicial
 * @param start_y Coordenada Y do ponto inicial
 * @param goal_x Coordenada X do objetivo
 * @param goal_y Coordenada Y do objetivo
 */
void a_star(int start_x, int start_y, int goal_x, int goal_y) {
    // Inicializa o nó de partida com custos apropriados
    grid[start_x][start_y].g = 0;
    grid[start_x][start_y].h = heuristic(start_x, start_y, goal_x, goal_y);
    grid[start_x][start_y].f = grid[start_x][start_y].h;
    grid[start_x][start_y].parent_x = -1;  // Nó inicial não tem pai
    grid[start_x][start_y].parent_y = -1;

    // Loop principal (limita o número de iterações por segurança)
    for (int step = 0; step < LINHAS * COLUNAS; step++) {
        int min_f = INT_MAX;
        int x = -1, y = -1;

        // Encontra o nó não visitado com o menor valor de f (custo total estimado)
        for (int i = 0; i < LINHAS; i++) {
            for (int j = 0; j < COLUNAS; j++) {
                if (!grid[i][j].visited && !grid[i][j].is_obstacle && grid[i][j].f < min_f) {
                    min_f = grid[i][j].f;
                    x = i;
                    y = j;
                }
            }
        }

        // Caso não haja mais nós disponíveis para explorar
        if (x == -1) {
             exportar_resultado("saidas/resultado_c.txt", start_x, start_y, goal_x, goal_y);  // Exporta o resultado encontrado até o momento
             sleep(20);  // Pausa para visualização
             return;
        }

        grid[x][y].visited = 1;  // Marca o nó atual como visitado
        salvar_frame(start_x, start_y, goal_x, goal_y);  // Salva o estado atual para visualização

        // Se o nó objetivo foi alcançado, reconstrói o caminho e encerra
        if (x == goal_x && y == goal_y) {
            mark_path_on_grid(start_x, start_y, goal_x, goal_y);  // Marca o caminho final
            exportar_resultado("saidas/resultado_c.txt", start_x, start_y, goal_x, goal_y);  // Exporta o resultado completo
            sleep(20);  // Pausa para permitir a visualização do resultado
            return;
        }

        // Vetores de deslocamento para os vizinhos (norte, sul, oeste, leste)
        int dx[4] = {-1, 1, 0, 0};
        int dy[4] = {0, 0, -1, 1};

        // Explora os vizinhos do nó atual
        for (int d = 0; d < 4; d++) {
            int nx = x + dx[d];
            int ny = y + dy[d];

            // Verifica se o vizinho está dentro dos limites da grade e não é um obstáculo
            if (nx >= 0 && nx < LINHAS && ny >= 0 && ny < COLUNAS && !grid[nx][ny].is_obstacle) {
                int new_g = grid[x][y].g + 1;  // Custo do caminho até o vizinho
                int new_h = heuristic(nx, ny, goal_x, goal_y);  // Estimativa até o objetivo
                int new_f = new_g + new_h;  // Novo custo total estimado

                // Atualiza o nó vizinho se encontrou um caminho melhor
                if (new_f < grid[nx][ny].f) {
                    grid[nx][ny].f = new_f;
                    grid[nx][ny].g = new_g;
                    grid[nx][ny].h = new_h;
                    grid[nx][ny].parent_x = x;  // Define o pai do vizinho
                    grid[nx][ny].parent_y = y;
                }
            }
        }
    }
}

// ================================
// Função principal (ponto de entrada do programa)
// ================================

/**
 * @brief Função principal que inicializa o ambiente e executa o algoritmo A*.
 */
void main() {

    // Inicializa todos os nós da grade com valores padrão
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            grid[i][j].f = INT_MAX;     // Inicializa custo total com valor máximo
            grid[i][j].is_obstacle = 0; // Define que não há obstáculo inicialmente
            grid[i][j].visited = 0;     // Marca todos os nós como não visitados
            grid[i][j].is_path = 0;     // Define que nenhum nó faz parte do caminho inicialmente
        }
    }

    // Define as coordenadas iniciais e finais para o caminho
    int start_x = 0, start_y = 0;   // Ponto inicial (canto superior esquerdo)
    int goal_x = 5, goal_y = 30;    // Ponto final (exemplo de destino)

    // Carrega o labirinto a partir de um arquivo de texto
    carregar_labirinto_de_arquivo("entradas/labirinto1.txt");

    // Executa o algoritmo A* para encontrar o caminho
    a_star(start_x, start_y, goal_x, goal_y);

    return;
}
