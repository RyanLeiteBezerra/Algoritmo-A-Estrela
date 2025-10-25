#include "util.h" // Incluímos nosso próprio cabeçalho com aspas duplas

int carregar_labirinto_de_arquivo(const char* filename) {
    FILE *f = fopen(filename, "r"); // "r" para modo de leitura (read)

    // Se o arquivo não puder ser aberto, retorna um erro
    if (f == NULL) {
        perror("Erro ao abrir o arquivo de labirinto");
        return 0; // Retorna 0 para indicar falha
    }

    printf("Carregando labirinto de '%s'...\n", filename);

    int cell_type;
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {

            if (fscanf(f, "%d", &cell_type) != 1) {
                printf("Erro: arquivo de labirinto inesperadamente curto ou mal formatado.\n");
                fclose(f);
                return 0; // Falha
            }

            if(cell_type == 1)
            grid[i][j].is_obstacle = 1;
        }
    }

    fclose(f);
    printf("Labirinto carregado com sucesso!\n");
    return 1; // Retorna 1 para indicar sucesso
}

void exportar_resultado(const char *filename, int start_x, int start_y, int goal_x, int goal_y) {
    FILE *f = fopen(filename, "w");
    if (f == NULL) {
        printf("Erro ao abrir o arquivo %s para escrita!\n", filename);
        return;
    }

    // Verifica se o destino foi alcançado
    if (!grid[goal_x][goal_y].is_path) {
        fprintf(f, "Nenhum caminho encontrado.\n");
        printf("Nenhum caminho encontrado.\n");
        fclose(f);
        return;
    }

    // Reconstrói o caminho
    int path_x[LINHAS * COLUNAS];
    int path_y[LINHAS * COLUNAS];
    int length = 0;
    int current_x = goal_x;
    int current_y = goal_y;

    while (!(current_x == start_x && current_y == start_y)) {
        path_x[length] = current_x;
        path_y[length] = current_y;
        int px = grid[current_x][current_y].parent_x;
        int py = grid[current_x][current_y].parent_y;
        current_x = px;
        current_y = py;
        length++;
    }
    path_x[length] = start_x;
    path_y[length] = start_y;
    length++;

    // Calcula métricas
    int custo_total = length - 1; // cada passo custa 1
    int nos_visitados = 0;

    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            if (grid[i][j].visited)
                nos_visitados++;
        }
    }

    // Escreve no arquivo e printa na tela
    fprintf(f, "Caminho encontrado!\n");
    fprintf(f, "Custo total: %d\n", custo_total);
    fprintf(f, "Nos visitados: %d\n", nos_visitados);
    fprintf(f, "Tamanho do caminho: %d\n", length);
    fprintf(f, "Caminho:\n");

    printf("Caminho encontrado!\n");
    printf("Custo total: %d\n", custo_total);
    printf("Nos visitados: %d\n", nos_visitados);
    printf("Tamanho do caminho: %d\n", length);
    printf("Caminho:\n");

    for (int i = length - 1; i >= 0; i--) {
        fprintf(f, "(%d,%d)", path_x[i], path_y[i]);
        printf("(%d,%d)", path_x[i], path_y[i]);
        if (i > 0) {
            fprintf(f, " -> ");
            printf(" -> ");
        }
    }

    fprintf(f, "\n");
    printf("\n");

    fclose(f);
}

void salvar_estado(const char *nome_arquivo, int start_x, int start_y, int goal_x, int goal_y) {
    FILE *f = fopen(nome_arquivo, "w");
    if (!f) {
        printf("Erro ao abrir %s\n", nome_arquivo);
        return;
    }

    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            int cell_state = 0; // 0: vazio

            if (i == start_x && j == start_y) {
                cell_state = 3; // Início
            } else if (i == goal_x && j == goal_y) {
                cell_state = 4; // Destino
            } else if (grid[i][j].is_path) {
                cell_state = 2; // Caminho final
            } else if (grid[i][j].visited) {
                cell_state = 5; // Visitado
            } else if (grid[i][j].is_obstacle) {
                cell_state = 1; // Obstáculo
            }

            fprintf(f, "%d ", cell_state);
        }
        fprintf(f, "\n");
    }

    fclose(f);
}

void salvar_frame(int start_x, int start_y, int goal_x, int goal_y) {
    char nome[64];
    sprintf(nome, "frames/frame_%04d.txt", frame_count++);
    salvar_estado(nome, start_x, start_y, goal_x, goal_y);
}

