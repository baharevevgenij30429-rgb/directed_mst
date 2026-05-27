#ifndef FIB_HEAP_H
#define FIB_HEAP_H

typedef struct FibNode {
    int vertex;
    long long key;
    struct FibNode *parent;
    struct FibNode *child;
    struct FibNode *left;
    struct FibNode *right;
    int degree;
    int mark;
} FibNode;

typedef struct FibHeap {
    FibNode *min;
    int n;
} FibHeap;

FibHeap* fib_heap_create(void);
void fib_heap_free(FibHeap *heap);
void fib_heap_insert(FibHeap *heap, int vertex, long long key);
FibNode* fib_heap_extract_min(FibHeap *heap);
int fib_heap_empty(FibHeap *heap);

#endif