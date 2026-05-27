#ifndef GABOW_TARJAN_H
#define GABOW_TARJAN_H

#include "graph.h"

// Алгоритм Габова-Тарьяна с фибоначчиевой кучей (O(m + n log n))
long long gabow_tarjan(Graph *g, int root, int *parent);

#endif