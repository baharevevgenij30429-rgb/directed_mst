#include "edmonds_simple.h"
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define INF (1LL<<60)  // "Бесконечность" для весов

// Локальная структура ребра для удобства
typedef struct {
    int from, to;
    long long w;
} MyEdge;

// Простой алгоритм Эдмондса (O(n·m))
long long edmonds_simple(Graph *g, int root, int *parent) {
    int n = g->n, m = g->m;
    
    // Копируем рёбра во временный массив
    MyEdge *edges = (MyEdge*)malloc(m * sizeof(MyEdge));
    for (int i = 0; i < m; i++) {
        edges[i].from = g->edges[i].from;
        edges[i].to = g->edges[i].to;
        edges[i].w = g->edges[i].weight;
    }
    
    int *pre = (int*)calloc(n, sizeof(int));   // предок в выбранном наборе
    long long *in = (long long*)calloc(n, sizeof(long long)); // вес входящего ребра
    int *id = (int*)malloc(n * sizeof(int));   // идентификатор сжатой вершины
    int *vis = (int*)malloc(n * sizeof(int));  // метка для поиска циклов
    long long total = 0;
    
    while (1) {
        // 1. Находим минимальное входящее ребро для каждой вершины
        for (int i = 0; i < n; i++) {
            if (i == root) {
                in[i] = 0;
                pre[i] = root;
                continue;
            }
            in[i] = INF;
            pre[i] = -1;
            for (int j = 0; j < m; j++) {
                if (edges[j].to == i && edges[j].from != i && edges[j].w < in[i]) {
                    in[i] = edges[j].w;
                    pre[i] = edges[j].from;
                }
            }
            if (pre[i] == -1) {  // нет входящего ребра → дерево построить нельзя
                total = -1;
                goto cleanup;
            }
        }
        
        // Добавляем веса выбранных рёбер к общей стоимости
        for (int i = 0; i < n; i++) total += in[i];
        
        // 2. Поиск циклов в массиве предков
        int cnt = 0;
        memset(id, -1, n * sizeof(int));
        memset(vis, -1, n * sizeof(int));
        for (int i = 0; i < n; i++) {
            int v = i;
            while (vis[v] != i && id[v] == -1 && v != root) {
                vis[v] = i;
                v = pre[v];
            }
            if (v != root && id[v] == -1) {
                // Нашли цикл: помечаем все вершины цикла одним id
                for (int u = pre[v]; u != v; u = pre[u]) id[u] = cnt;
                id[v] = cnt++;
            }
        }
        
        // 3. Если циклов нет — сохраняем результат
        if (cnt == 0) {
            for (int i = 0; i < n; i++) parent[i] = pre[i];
            break;
        }
        
        // Присваиваем id остальным вершинам
        for (int i = 0; i < n; i++) if (id[i] == -1) id[i] = cnt++;
        
        // 4. Контракция графа (сжатие циклов в одну вершину)
        int new_m = 0;
        MyEdge *new_edges = (MyEdge*)malloc(m * sizeof(MyEdge));
        for (int i = 0; i < m; i++) {
            int u = edges[i].from, v = edges[i].to;
            int nu = id[u], nv = id[v];
            if (nu != nv) {
                new_edges[new_m].from = nu;
                new_edges[new_m].to = nv;
                new_edges[new_m].w = edges[i].w - in[v];
                new_m++;
            }
        }
        free(edges);
        edges = new_edges;
        m = new_m;
        n = cnt;
        root = id[root];
    }
    
cleanup:
    free(edges);
    free(pre);
    free(in);
    free(id);
    free(vis);
    return total;
}