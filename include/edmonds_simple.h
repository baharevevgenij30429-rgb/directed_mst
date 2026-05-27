#ifndef EDMONDS_SIMPLE_H
#define EDMONDS_SIMPLE_H

#include "graph.h"

// Простой алгоритм Эдмондса (O(n·m))
// Возвращает стоимость дерева или -1, если построить нельзя
// В parent записываются предки для каждой вершины
long long edmonds_simple(Graph *g, int root, int *parent);

#endif