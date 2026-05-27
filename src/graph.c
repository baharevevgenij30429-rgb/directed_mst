#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Создаёт граф с n вершинами и m рёбрами
Graph* graph_create(int n, int m) {
    Graph *g = (Graph*)malloc(sizeof(Graph));
    g->n = n;
    g->m = m;
    g->edges = (Edge*)malloc(m * sizeof(Edge));
    return g;
}

// Освобождает память графа
void graph_free(Graph *g) {
    if (g) {
        free(g->edges);
        free(g);
    }
}

// Добавляет ребро по индексу
void graph_add_edge(Graph *g, int idx, int from, int to, long long w) {
    g->edges[idx].from = from;
    g->edges[idx].to = to;
    g->edges[idx].weight = w;
}

// Читает граф из текстового файла
Graph* graph_read(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return NULL;
    int n, m;
    fscanf(f, "%d %d", &n, &m);
    Graph *g = graph_create(n, m);
    for (int i = 0; i < m; i++) {
        int u, v; long long w;
        fscanf(f, "%d %d %lld", &u, &v, &w);
        graph_add_edge(g, i, u, v, w);
    }
    fclose(f);
    return g;
}

// Записывает результат в файл
void graph_write_result(const char *filename, long long total_cost, int *parent, int n, int root) {
    FILE *f = fopen(filename, "w");
    if (!f) return;
    if (total_cost == -1) {
        fprintf(f, "NO\n");
    } else {
        fprintf(f, "YES\n%lld\n", total_cost);
        for (int i = 0; i < n; i++) {
            if (i != root && parent[i] != -1) {
                fprintf(f, "%d %d\n", parent[i], i);
            }
        }
    }
    fclose(f);
}

// Проверяет достижимость всех вершин из корня (обход в ширину)
int graph_is_connected(Graph *g, int root) {
    int *visited = (int*)calloc(g->n, sizeof(int));
    int *queue = (int*)malloc(g->n * sizeof(int));
    int head = 0, tail = 0;
    
    visited[root] = 1;
    queue[tail++] = root;
    
    // Строим списки смежности для BFS
    int **adj = (int**)malloc(g->n * sizeof(int*));
    int *deg = (int*)calloc(g->n, sizeof(int));
    for (int i = 0; i < g->m; i++) deg[g->edges[i].from]++;
    for (int i = 0; i < g->n; i++) adj[i] = (int*)malloc(deg[i] * sizeof(int));
    int *pos = (int*)calloc(g->n, sizeof(int));
    for (int i = 0; i < g->m; i++) {
        int u = g->edges[i].from, v = g->edges[i].to;
        adj[u][pos[u]++] = v;
    }
    
    // Сам BFS
    while (head < tail) {
        int u = queue[head++];
        for (int i = 0; i < deg[u]; i++) {
            int v = adj[u][i];
            if (!visited[v]) {
                visited[v] = 1;
                queue[tail++] = v;
            }
        }
    }
    
    // Проверяем, все ли посетили
    int ok = 1;
    for (int i = 0; i < g->n; i++) {
        if (!visited[i]) { ok = 0; break; }
    }
    
    // Освобождаем память
    free(visited);
    free(queue);
    for (int i = 0; i < g->n; i++) free(adj[i]);
    free(adj);
    free(deg);
    free(pos);
    
    return ok;
}