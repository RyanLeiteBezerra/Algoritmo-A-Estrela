#include "util.h"

// Grid global
Node grid[LINHAS][COLUNAS];

//Contador de Frames
int frame_count = 0; 

// Função heurística (Distância de Manhattan)
int heuristic(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

// Função para marcar os nós que pertencem ao caminho final no grid
void mark_path_on_grid(int start_x, int start_y, int goal_x, int goal_y) {
    int current_x = goal_x;
    int current_y = goal_y;

    // Percorre de trás para frente, do destino ao início
    while (current_x != start_x || current_y != start_y) {
        grid[current_x][current_y].is_path = 1;
        int temp_x = grid[current_x][current_y].parent_x;
        int temp_y = grid[current_x][current_y].parent_y;
        current_x = temp_x;
        current_y = temp_y;

        salvar_frame(start_x, start_y, goal_x, goal_y);
    }
    // Marca o nó inicial também
    grid[start_x][start_y].is_path = 1;
}

// Função principal do A*
void a_star(int start_x, int start_y, int goal_x, int goal_y) {
    // Inicialização do nó de partida
    grid[start_x][start_y].g = 0;
    grid[start_x][start_y].h = heuristic(start_x, start_y, goal_x, goal_y);
    grid[start_x][start_y].f = grid[start_x][start_y].h;
    grid[start_x][start_y].parent_x = -1;
    grid[start_x][start_y].parent_y = -1;

    // Loop principal (limitado para segurança)
    for (int step = 0; step < LINHAS * COLUNAS; step++) {
        int min_f = INT_MAX;
        int x = -1, y = -1;

        // Encontra o nó não visitado com o menor custo f
        for (int i = 0; i < LINHAS; i++) {
            for (int j = 0; j < COLUNAS; j++) {
                if (!grid[i][j].visited && !grid[i][j].is_obstacle && grid[i][j].f < min_f) {
                    min_f = grid[i][j].f;
                    x = i;
                    y = j;
                }
            }
        }

        if (x == -1) { // Se não há mais nós para explorar
             exportar_resultado("saidas/resultado_c.txt",start_x,start_y,goal_x,goal_y);
             sleep(20);
             return;
        }
        
        grid[x][y].visited = 1; 
        salvar_frame(start_x, start_y, goal_x, goal_y); 

        // Se encontrou o destino
        if (x == goal_x && y == goal_y) {
            mark_path_on_grid(start_x, start_y, goal_x, goal_y);
            exportar_resultado("saidas/resultado_c.txt",start_x,start_y,goal_x,goal_y);
            sleep(20);
            return; 
        }

        // Explora os vizinhos (4 direções)
        int dx[4] = {-1, 1, 0, 0};
        int dy[4] = {0, 0, -1, 1};

        for (int d = 0; d < 4; d++) {
            int nx = x + dx[d];
            int ny = y + dy[d];
            
            // Verifica se o vizinho está dentro da grade e não é um obstáculo
            if (nx >= 0 && nx < LINHAS && ny >= 0 && ny < COLUNAS && !grid[nx][ny].is_obstacle) {
                int new_g = grid[x][y].g + 1;
                int new_h = heuristic(nx, ny, goal_x, goal_y);
                int new_f = new_g + new_h;

                // Se um caminho melhor foi encontrado para este vizinho
                if (new_f < grid[nx][ny].f) {
                    grid[nx][ny].f = new_f;
                    grid[nx][ny].g = new_g;
                    grid[nx][ny].h = new_h;
                    grid[nx][ny].parent_x = x;
                    grid[nx][ny].parent_y = y;
                }
            }
        }
    }
}

void main() {

    // Inicializa toda a grade com valores padrão
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            grid[i][j].f = INT_MAX;
            grid[i][j].is_obstacle = 0;
            grid[i][j].visited = 0;
            grid[i][j].is_path = 0; // Importante inicializar como 0
        }
    }

    // Inicializa os pontos de início e fim
    int start_x = 0, start_y = 0, goal_x = 5, goal_y = 30;

    //Carregar os obstáculos
    carregar_labirinto_de_arquivo("entradas/labirinto1.txt");

    //Executa o algoritmo A*
    a_star(start_x, start_y, goal_x, goal_y);
    
    return;
}