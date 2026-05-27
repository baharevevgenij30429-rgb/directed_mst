#ifndef HEAP_H
#define HEAP_H

#include <stddef.h>

typedef struct HeapNode {
    int vertex;
    long long key;
} HeapNode;

typedef struct Heap {
    HeapNode *data;
    int *pos;
    int size, cap;
} Heap;

Heap* heap_create(int max_vertices);
void heap_free(Heap *h);
int heap_empty(Heap *h);
void heap_push(Heap *h, int v, long long key);
HeapNode heap_pop(Heap *h);

#endif