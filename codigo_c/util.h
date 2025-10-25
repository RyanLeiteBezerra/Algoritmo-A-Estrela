#ifndef UTIL_H_
#define UTIL_H_

#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <unistd.h>

#define LINHAS 32
#define COLUNAS 32

typedef struct {
    int parent_x, parent_y;
    int g, h, f;
    int is_obstacle;
    int visited;
    int is_path;
} Node;

extern Node grid[LINHAS][COLUNAS];
extern int frame_count;

int carregar_labirinto_de_arquivo(const char* filename);
void exportar_resultado(const char *filename, int start_x, int start_y, int goal_x, int goal_y);
void salvar_estado(const char *nome_arquivo, int start_x, int start_y, int goal_x, int goal_y);
void salvar_frame(int start_x, int start_y, int goal_x, int goal_y);

#endif // UTIL_H_