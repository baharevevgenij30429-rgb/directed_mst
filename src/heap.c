#include "heap.h"
#include <stdlib.h>

Heap* heap_create(int max_vertices) {
    Heap *h = (Heap*)malloc(sizeof(Heap));
    h->cap = max_vertices + 5;
    h->size = 0;
    h->data = (HeapNode*)malloc(h->cap * sizeof(HeapNode));
    h->pos = (int*)malloc(max_vertices * sizeof(int));
    for (int i = 0; i < max_vertices; i++) h->pos[i] = -1;
    return h;
}

void heap_free(Heap *h) { free(h->data); free(h->pos); free(h); }
int heap_empty(Heap *h) { return h->size == 0; }

static void heap_up(Heap *h, int idx) {
    while (idx > 0) {
        int p = (idx - 1) / 2;
        if (h->data[p].key <= h->data[idx].key) break;
        HeapNode tmp = h->data[p];
        h->data[p] = h->data[idx];
        h->data[idx] = tmp;
        h->pos[h->data[p].vertex] = p;
        h->pos[h->data[idx].vertex] = idx;
        idx = p;
    }
}

static void heap_down(Heap *h, int idx) {
    while (1) {
        int l = idx * 2 + 1, r = idx * 2 + 2, smallest = idx;
        if (l < h->size && h->data[l].key < h->data[smallest].key) smallest = l;
        if (r < h->size && h->data[r].key < h->data[smallest].key) smallest = r;
        if (smallest == idx) break;
        HeapNode tmp = h->data[idx];
        h->data[idx] = h->data[smallest];
        h->data[smallest] = tmp;
        h->pos[h->data[idx].vertex] = idx;
        h->pos[h->data[smallest].vertex] = smallest;
        idx = smallest;
    }
}

void heap_push(Heap *h, int v, long long key) {
    h->data[h->size].vertex = v;
    h->data[h->size].key = key;
    h->pos[v] = h->size;
    h->size++;
    heap_up(h, h->size - 1);
}

HeapNode heap_pop(Heap *h) {
    HeapNode res = h->data[0];
    h->pos[res.vertex] = -1;
    h->size--;
    if (h->size > 0) {
        h->data[0] = h->data[h->size];
        h->pos[h->data[0].vertex] = 0;
        heap_down(h, 0);
    }
    return res;
}