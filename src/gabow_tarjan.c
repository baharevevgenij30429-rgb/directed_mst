#include "gabow_tarjan.h"
#include "fib_heap.h"
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define INF (1LL<<60)

typedef struct {
    int from, to;
    long long w;
} MyEdge;

// Алгоритм Габова-Тарьяна с фибоначчиевой кучей (O(m + n log n))
long long gabow_tarjan(Graph *g, int root, int *parent) {
    int n = g->n, m = g->m;
    
    MyEdge *edges = (MyEdge*)malloc(m * sizeof(MyEdge));
    for (int i = 0; i < m; i++) {
        edges[i].from = g->edges[i].from;
        edges[i].to = g->edges[i].to;
        edges[i].w = g->edges[i].weight;
    }
    
    int *pre = (int*)calloc(n, sizeof(int));
    long long *in = (long long*)calloc(n, sizeof(long long));
    int *id = (int*)malloc(n * sizeof(int));
    int *vis = (int*)malloc(n * sizeof(int));
    long long total = 0;
    
    while (1) {
        // Создаём фибоначчиевы кучи входящих рёбер
        FibHeap **heaps = (FibHeap**)malloc(n * sizeof(FibHeap*));
        for (int i = 0; i < n; i++) heaps[i] = fib_heap_create();
        for (int i = 0; i < m; i++) {
            fib_heap_insert(heaps[edges[i].to], edges[i].from, edges[i].w);
        }
        
        // Выбираем минимальное входящее ребро из фибоначчиевой кучи
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
            in[i] = node->key;
            pre[i] = node->vertex;
            free(node);
        }
        
        for (int i = 0; i < n; i++) total += in[i];
        
        // Поиск циклов (как в простой версии)
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
                for (int u = pre[v]; u != v; u = pre[u]) id[u] = cnt;
                id[v] = cnt++;
            }
        }
        
        if (cnt == 0) {
            for (int i = 0; i < n; i++) parent[i] = pre[i];
            for (int j = 0; j < n; j++) fib_heap_free(heaps[j]);
            free(heaps);
            break;
        }
        
        for (int i = 0; i < n; i++) if (id[i] == -1) id[i] = cnt++;
        
        // Контракция
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