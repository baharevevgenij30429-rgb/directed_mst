#ifndef GRAPH_H
#define GRAPH_H

#include <stddef.h>

// Структура ребра графа
typedef struct Edge {
    int from;          // откуда идёт ребро
    int to;            // куда идёт ребро
    long long weight;  // вес (стоимость) ребра
} Edge;

// Структура графа
typedef struct Graph {
    int n;             // количество вершин
    int m;             // количество рёбер
    Edge *edges;       // массив рёбер
} Graph;

// Создаёт граф с n вершинами и m рёбрами
Graph* graph_create(int n, int m);

// Освобождает память, занятую графом
void graph_free(Graph *g);

// Добавляет ребро в граф по индексу
void graph_add_edge(Graph *g, int idx, int from, int to, long long w);

// Читает граф из файла
Graph* graph_read(const char *filename);

// Записывает результат (найденное дерево) в файл
void graph_write_result(const char *filename, long long total_cost, int *parent, int n, int root);

// Проверяет, все ли вершины достижимы из корня (обход в ширину)
int graph_is_connected(Graph *g, int root);

#endif