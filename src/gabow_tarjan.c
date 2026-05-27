#include "gabow_tarjan.h"
#include "fib_heap.h"
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define INF (1LL<<60)

long long gabow_tarjan(Graph *g, int root, int *parent) {
    int n = g->n;
    int m = g->m;
    
    // Копия рёбер
    Edge *edges = (Edge*)malloc(m * sizeof(Edge));
    memcpy(edges, g->edges, m * sizeof(Edge));
    
    int *pre = (int*)calloc(n, sizeof(int));
    long long *in = (long long*)calloc(n, sizeof(long long));
    int *id = (int*)malloc(n * sizeof(int));
    int *vis = (int*)malloc(n * sizeof(int));
    long long total = 0;
    
    while (1) {
        // Создаём фибоначчиевы кучи для каждой вершины
        FibHeap **heaps = (FibHeap**)malloc(n * sizeof(FibHeap*));
        for (int i = 0; i < n; i++) {
            heaps[i] = fib_heap_create();
        }
        
        // Вставляем все рёбра в кучи (по вершине назначения)
        for (int i = 0; i < m; i++) {
            fib_heap_insert(heaps[edges[i].to], edges[i].from, edges[i].weight);
        }
        
        // Выбираем минимальное входящее ребро для каждой вершины (кроме корня)
        for (int i = 0; i < n; i++) {
            if (i == root) {
                in[i] = 0;
                pre[i] = root;
                continue;
            }
            if (fib_heap_empty(heaps[i])) {
                total = -1;
                for (int j = 0; j < n; j++) fib_heap_free(heaps[j]);
                free(heaps);
                goto cleanup;
            }
            FibNode *node = fib_heap_extract_min(heaps[i]);
            if (!node) {
                total = -1;
                for (int j = 0; j < n; j++) fib_heap_free(heaps[j]);
                free(heaps);
                goto cleanup;
            }
            in[i] = node->key;
            pre[i] = node->vertex;
            free(node);
        }
        
        // Суммируем стоимости выбранных рёбер
        for (int i = 0; i < n; i++) {
            total += in[i];
        }
        
        // Поиск циклов (алгоритм на основе меток)
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
                for (int u = pre[v]; u != v; u = pre[u]) {
                    id[u] = cnt;
                }
                id[v] = cnt++;
            }
        }
        
        // Если циклов нет – сохраняем предков и выходим
        if (cnt == 0) {
            for (int i = 0; i < n; i++) {
                parent[i] = pre[i];
            }
            for (int j = 0; j < n; j++) fib_heap_free(heaps[j]);
            free(heaps);
            break;
        }
        
        // Присваиваем ID вершинам, не входящим в циклы
        for (int i = 0; i < n; i++) {
            if (id[i] == -1) id[i] = cnt++;
        }
        
        // Контракция графа
        int new_m = 0;
        Edge *new_edges = (Edge*)malloc(m * sizeof(Edge));
        for (int i = 0; i < m; i++) {
            int u = edges[i].from;
            int v = edges[i].to;
            int nu = id[u];
            int nv = id[v];
            if (nu != nv) {
                new_edges[new_m].from = nu;
                new_edges[new_m].to = nv;
                new_edges[new_m].weight = edges[i].weight - in[v];
                new_m++;
            }
        }
        free(edges);
        edges = new_edges;
        m = new_m;
        n = cnt;
        root = id[root];
        
        // Очищаем кучи и продолжаем
        for (int j = 0; j < n; j++) fib_heap_free(heaps[j]);
        free(heaps);
    }
    
cleanup:
    free(edges);
    free(pre);
    free(in);
    free(id);
    free(vis);
    return total;
}