import numpy as np
import heapq

def heuristic(a, b):
    """Distância de Manhattan"""
    return abs(a[0] - b[0]) + abs(a[1] - b[1])

def load_grid(filename):
    """Lê o grid do arquivo texto"""
    grid = np.loadtxt(filename, dtype=int)
    start = tuple(np.argwhere(grid == 3)[0])
    goal = tuple(np.argwhere(grid == 4)[0])
    return grid, start, goal

def a_star(grid, start, goal):
    """Executa o algoritmo A*"""
    linhas, colunas = grid.shape
    open_set = []
    heapq.heappush(open_set, (0, start))
    g_score = {start: 0}
    came_from = {}
    visited = set()

    while open_set:
        _, current = heapq.heappop(open_set)
        visited.add(current)

        if current == goal:
            # Reconstrói o caminho
            path = []
            while current in came_from:
                path.append(current)
                current = came_from[current]
            path.append(start)
            path.reverse()
            return path, len(visited), g_score[goal]

        for dx, dy in [(-1,0),(1,0),(0,-1),(0,1)]:
            nx, ny = current[0] + dx, current[1] + dy
            if 0 <= nx < linhas and 0 <= ny < colunas:
                if grid[nx, ny] == 1:
                    continue  # Obstáculo
                neighbor = (nx, ny)
                tentative_g = g_score[current] + 1
                if tentative_g < g_score.get(neighbor, float('inf')):
                    came_from[neighbor] = current
                    g_score[neighbor] = tentative_g
                    f = tentative_g + heuristic(neighbor, goal)
                    heapq.heappush(open_set, (f, neighbor))
    return None, len(visited), None

def main():
    grid, start, goal = load_grid("validador/entradas/labirinto1.txt")
    path, visited_count, total_cost = a_star(grid, start, goal)

    with open("validador/saidas/resultado_validador.txt", "w") as f:
        if path:
            f.write("Caminho encontrado!\n")
            f.write(f"Custo total: {total_cost}\n")
            f.write(f"Nos visitados: {visited_count}\n")
            f.write(f"Tamanho do caminho: {len(path)}\n")
            f.write("Caminho:\n")
            f.write(" -> ".join([f"({x},{y})" for x,y in path]))
        else:
            f.write("Nenhum caminho encontrado.\n")
            f.write(f"Nós visitados: {visited_count}\n")

    print("✅ Validação concluída! Resultados salvos em 'resultado_validador.txt'")

if __name__ == "__main__":
    main()