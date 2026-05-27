#ifndef GRAPH_H
#define GRAPH_H

#include <stddef.h>

typedef struct Edge {
    int from;          // откуда
    int to;            // куда
    long long weight;  // вес (пропускная способность или стоимость)
} Edge;

typedef struct Graph {
    int n;             // количество вершин
    int m;             // количество рёбер
    Edge *edges;       // массив рёбер
} Graph;

// Создание графа
Graph* graph_create(int n, int m);

// Удаление графа (освобождение памяти)
void graph_free(Graph *g);

// Добавление ребра (по индексу)
void graph_add_edge(Graph *g, int idx, int from, int to, long long w);

// Чтение графа из файла
Graph* graph_read(const char *filename);

// Запись результата (дерева) в файл
void graph_write_result(const char *filename, long long total_cost, int *parent, int n, int root);

// Проверка, что все вершины достижимы из корня (BFS)
int graph_is_connected(Graph *g, int root);

#endif