#include "util.h" // Incluímos nosso próprio cabeçalho com aspas duplas, pois ele está no diretório local e contém definições e funções auxiliares necessárias

// =============================================================
// Função: carregar_labirinto_de_arquivo
// -------------------------------------------------------------
// Lê um arquivo de texto contendo a representação numérica de um labirinto
// e carrega as informações na matriz global "grid".
// Cada número representa o tipo de célula (0 = livre, 1 = obstáculo).
// =============================================================
int carregar_labirinto_de_arquivo(const char* filename) {
    FILE *f = fopen(filename, "r"); // Abre o arquivo em modo leitura ("r")

    // Se o arquivo não pôde ser aberto, exibe mensagem de erro e retorna falha
    if (f == NULL) {
        perror("Erro ao abrir o arquivo de labirinto");
        return 0; // Retorna 0 para indicar falha
    }

    printf("Carregando labirinto de '%s'...\n", filename);

    int cell_type; // Variável temporária para armazenar o tipo da célula (lido do arquivo)

    // Percorre toda a matriz do grid (linha por linha, coluna por coluna)
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {

            // Lê um número do arquivo. Se falhar, significa arquivo mal formatado ou incompleto
            if (fscanf(f, "%d", &cell_type) != 1) {
                printf("Erro: arquivo de labirinto inesperadamente curto ou mal formatado.\n");
                fclose(f);
                return 0; // Falha na leitura
            }

            // Marca a célula como obstáculo se o valor for 1
            if (cell_type == 1)
                grid[i][j].is_obstacle = 1;
        }
    }

    fclose(f); // Fecha o arquivo após a leitura
    printf("Labirinto carregado com sucesso!\n");
    return 1; // Retorna 1 indicando sucesso na operação
}

// =============================================================
// Função: exportar_resultado
// -------------------------------------------------------------
// Gera um relatório com os resultados do algoritmo A*.
// Inclui o custo do caminho, número de nós visitados e o caminho encontrado.
// O resultado é salvo em um arquivo e também impresso no console.
// =============================================================
void exportar_resultado(const char *filename, int start_x, int start_y, int goal_x, int goal_y) {
    FILE *f = fopen(filename, "w"); // Abre o arquivo para escrita ("w" sobrescreve)
    if (f == NULL) {
        printf("Erro ao abrir o arquivo %s para escrita!\n", filename);
        return;
    }

    // Se o destino não foi alcançado (nenhum caminho encontrado)
    if (!grid[goal_x][goal_y].is_path) {
        fprintf(f, "Nenhum caminho encontrado.\n");
        printf("Nenhum caminho encontrado.\n");
        fclose(f);
        return;
    }

    // Vetores auxiliares para armazenar as coordenadas do caminho reconstruído
    int path_x[LINHAS * COLUNAS];
    int path_y[LINHAS * COLUNAS];
    int length = 0; // Comprimento atual do caminho

    int current_x = goal_x;
    int current_y = goal_y;

    // Reconstrói o caminho percorrendo os pais até chegar ao início
    while (!(current_x == start_x && current_y == start_y)) {
        path_x[length] = current_x;
        path_y[length] = current_y;

        // Recupera o nó pai (o nó anterior no caminho)
        int px = grid[current_x][current_y].parent_x;
        int py = grid[current_x][current_y].parent_y;

        current_x = px;
        current_y = py;
        length++;
    }

    // Inclui o nó inicial no caminho reconstruído
    path_x[length] = start_x;
    path_y[length] = start_y;
    length++;

    // =============================================================
    // Cálculo de métricas
    // =============================================================

    int custo_total = length - 1; // Cada passo tem custo unitário
    int nos_visitados = 0;        // Contador de nós que foram explorados

    // Conta quantos nós foram visitados durante a execução do algoritmo
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            if (grid[i][j].visited)
                nos_visitados++;
        }
    }

    // =============================================================
    // Impressão e escrita dos resultados
    // =============================================================

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

    // Imprime o caminho do início ao fim
    for (int i = length - 1; i >= 0; i--) {
        fprintf(f, "(%d,%d)", path_x[i], path_y[i]);
        printf("(%d,%d)", path_x[i], path_y[i]);

        // Adiciona um separador visual "->" entre as coordenadas
        if (i > 0) {
            fprintf(f, " -> ");
            printf(" -> ");
        }
    }

    fprintf(f, "\n");
    printf("\n");

    fclose(f); // Fecha o arquivo de saída
}

// =============================================================
// Função: salvar_estado
// -------------------------------------------------------------
// Salva o estado atual do grid em um arquivo de texto.
// Cada célula é representada por um número conforme seu estado atual:
//   0 - Espaço vazio
//   1 - Obstáculo
//   2 - Parte do caminho final
//   3 - Ponto de início
//   4 - Ponto de destino
//   5 - Nó visitado pelo algoritmo
// =============================================================
void salvar_estado(const char *nome_arquivo, int start_x, int start_y, int goal_x, int goal_y) {
    FILE *f = fopen(nome_arquivo, "w"); // Abre arquivo em modo escrita
    if (!f) {
        printf("Erro ao abrir %s\n", nome_arquivo);
        return;
    }

    // Percorre toda a grade e salva o estado de cada célula
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            int cell_state = 0; // Valor padrão (vazio)

            // Define o tipo da célula conforme sua função ou estado atual
            if (i == start_x && j == start_y) {
                cell_state = 3; // Ponto inicial
            } else if (i == goal_x && j == goal_y) {
                cell_state = 4; // Ponto de destino
            } else if (grid[i][j].is_path) {
                cell_state = 2; // Célula faz parte do caminho final
            } else if (grid[i][j].visited) {
                cell_state = 5; // Célula já visitada
            } else if (grid[i][j].is_obstacle) {
                cell_state = 1; // Célula bloqueada (obstáculo)
            }

            fprintf(f, "%d ", cell_state); // Escreve o valor correspondente no arquivo
        }
        fprintf(f, "\n"); // Nova linha após cada linha da grade
    }

    fclose(f); // Fecha o arquivo
}

// =============================================================
// Função: salvar_frame
// -------------------------------------------------------------
// Cria um arquivo de estado numerado para cada frame de execução do A*.
// Cada frame representa o estado atual do grid em um determinado momento.
// =============================================================
void salvar_frame(int start_x, int start_y, int goal_x, int goal_y) {
    char nome[64]; // Buffer para armazenar o nome do arquivo
    sprintf(nome, "frames/frame_%04d.txt", frame_count++); // Gera nome sequencial (ex: frame_0001.txt)
    salvar_estado(nome, start_x, start_y, goal_x, goal_y); // Salva o estado atual do grid
}