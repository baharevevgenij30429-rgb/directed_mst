#include "edmonds_simple.h"
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define INF (1LL<<60)

long long edmonds_simple(Graph *g, int root, int *parent) {
    int n = g->n;
    long long total = 0;
    int *pre = (int*)malloc(n * sizeof(int));
    long long *in = (long long*)malloc(n * sizeof(long long));
    int *id = (int*)malloc(n * sizeof(int));
    int *vis = (int*)malloc(n * sizeof(int));
    
    Edge *edges = (Edge*)malloc(g->m * sizeof(Edge));
    memcpy(edges, g->edges, g->m * sizeof(Edge));
    int m = g->m;
    
    while (1) {
        // find min incoming edge
        for (int i = 0; i < n; i++) {
            if (i == root) {
                in[i] = 0; pre[i] = root;
                continue;
            }
            in[i] = INF; pre[i] = -1;
            for (int j = 0; j < m; j++) {
                if (edges[j].to == i && edges[j].from != i && edges[j].weight < in[i]) {
                    in[i] = edges[j].weight;
                    pre[i] = edges[j].from;
                }
            }
            if (pre[i] == -1) { total = -1; goto cleanup; }
        }
        
        for (int i = 0; i < n; i++) total += in[i];
        
        // find cycles
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
            break;
        }
        for (int i = 0; i < n; i++) if (id[i] == -1) id[i] = cnt++;
        
        // contract
        int new_m = 0;
        Edge *new_edges = (Edge*)malloc(m * sizeof(Edge));
        for (int i = 0; i < m; i++) {
            int u = edges[i].from, v = edges[i].to;
            long long w = edges[i].weight;
            int nu = id[u], nv = id[v];
            if (nu != nv) {
                new_edges[new_m].from = nu;
                new_edges[new_m].to = nv;
                new_edges[new_m].weight = w - in[v];
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
    free(pre); free(in); free(id); free(vis); free(edges);
    return total;
}