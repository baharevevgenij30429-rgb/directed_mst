#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "graph.h"
#include "edmonds_simple.h"
#include "edmonds_opt.h"
#include "gabow_tarjan.h"

int main(int argc, char **argv) {
    if (argc < 2) { fprintf(stderr, "Usage: %s <input_file> [root]\n", argv[0]); return 1; }
    Graph *g = graph_read(argv[1]);
    if (!g) { fprintf(stderr, "Failed to read graph\n"); return 1; }
    int root = (argc >= 3) ? atoi(argv[2]) : 0;
    if (root < 0 || root >= g->n) { fprintf(stderr, "Invalid root\n"); return 1; }
    
    if (!graph_is_connected(g, root)) {
        printf("NO\n");
        graph_write_result("output_simple.txt", -1, NULL, g->n, root);
        graph_write_result("output_opt.txt", -1, NULL, g->n, root);
        graph_write_result("output_gabow.txt", -1, NULL, g->n, root);
        graph_free(g);
        return 0;
    }
    
    int *p1 = malloc(g->n * sizeof(int));
    int *p2 = malloc(g->n * sizeof(int));
    int *p3 = malloc(g->n * sizeof(int));
    clock_t start, end;
    
    printf("Running simple Edmonds...\n");
    start = clock();
    long long c1 = edmonds_simple(g, root, p1);
    end = clock();
    printf("Simple: %lld, time: %.3f ms\n", c1, (double)(end-start)*1000/CLOCKS_PER_SEC);
    
    printf("Running optimized Edmonds (heap)...\n");
    start = clock();
    long long c2 = edmonds_opt(g, root, p2);
    end = clock();
    printf("Optimized: %lld, time: %.3f ms\n", c2, (double)(end-start)*1000/CLOCKS_PER_SEC);
    
    printf("Running Gabow-Tarjan...\n");
    start = clock();
    long long c3 = gabow_tarjan(g, root, p3);
    end = clock();
    printf("Gabow-Tarjan: %lld, time: %.3f ms\n", c3, (double)(end-start)*1000/CLOCKS_PER_SEC);
    
    graph_write_result("output_simple.txt", c1, p1, g->n, root);
    graph_write_result("output_opt.txt", c2, p2, g->n, root);
    graph_write_result("output_gabow.txt", c3, p3, g->n, root);
    
    free(p1); free(p2); free(p3);
    graph_free(g);
    return 0;
}