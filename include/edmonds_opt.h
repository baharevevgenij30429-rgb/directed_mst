#ifndef EDMONDS_OPT_H
#define EDMONDS_OPT_H

#include "graph.h"

// Оптимизированный алгоритм Эдмондса с бинарной кучей (O(m log n))
long long edmonds_opt(Graph *g, int root, int *parent);

#endif