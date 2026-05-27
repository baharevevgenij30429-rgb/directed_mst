#include "fib_heap.h"
#include <stdlib.h>

// Вспомогательная функция: добавляет узел в корневой список
static void fib_heap_add_to_root_list(FibHeap *heap, FibNode *node) {
    if (heap->min == NULL) {
        node->left = node;
        node->right = node;
        heap->min = node;
    } else {
        node->left = heap->min;
        node->right = heap->min->right;
        heap->min->right->left = node;
        heap->min->right = node;
    }
}

// Вспомогательная функция: удаляет узел из корневого списка
static void fib_heap_remove_from_root_list(FibHeap *heap, FibNode *node) {
    if (node == heap->min && node->right == node) {
        heap->min = NULL;
    } else {
        node->left->right = node->right;
        node->right->left = node->left;
        if (node == heap->min) heap->min = node->right;
    }
    node->left = node->right = node;
}

// Связывает два узла (y становится ребёнком x)
static void fib_heap_link(FibHeap *heap, FibNode *y, FibNode *x) {
    fib_heap_remove_from_root_list(heap, y);
    y->parent = x;
    if (x->child == NULL) {
        x->child = y;
        y->left = y->right = y;
    } else {
        y->left = x->child;
        y->right = x->child->right;
        x->child->right->left = y;
        x->child->right = y;
    }
    x->degree++;
    y->mark = 0;
}

// Уплотнение кучи (слияние деревьев одинаковой степени)
static void fib_heap_consolidate(FibHeap *heap) {
    int D = 64;
    FibNode **A = (FibNode**)calloc(D, sizeof(FibNode*));
    FibNode *w = heap->min;
    do {
        FibNode *x = w;
        int d = x->degree;
        while (A[d] != NULL) {
            FibNode *y = A[d];
            if (x->key > y->key) {
                FibNode *t = x; x = y; y = t;
            }
            fib_heap_link(heap, y, x);
            A[d] = NULL;
            d++;
        }
        A[d] = x;
        w = w->right;
    } while (w != heap->min);
    
    heap->min = NULL;
    for (int i = 0; i < D; i++) {
        if (A[i] != NULL) {
            if (heap->min == NULL || A[i]->key < heap->min->key)
                heap->min = A[i];
            fib_heap_add_to_root_list(heap, A[i]);
        }
    }
    free(A);
}

// Создаёт пустую фибоначчиеву кучу
FibHeap* fib_heap_create(void) {
    FibHeap *heap = (FibHeap*)malloc(sizeof(FibHeap));
    heap->min = NULL;
    heap->n = 0;
    return heap;
}

// Освобождает память (упрощённо)
void fib_heap_free(FibHeap *heap) {
    free(heap);
}

// Вставляет новый узел
void fib_heap_insert(FibHeap *heap, int vertex, long long key) {
    FibNode *node = (FibNode*)malloc(sizeof(FibNode));
    node->vertex = vertex;
    node->key = key;
    node->parent = NULL;
    node->child = NULL;
    node->left = node->right = node;
    node->degree = 0;
    node->mark = 0;
    fib_heap_add_to_root_list(heap, node);
    if (heap->min == NULL || key < heap->min->key)
        heap->min = node;
    heap->n++;
}

// Извлекает минимальный элемент
FibNode* fib_heap_extract_min(FibHeap *heap) {
    FibNode *z = heap->min;
    if (z == NULL) return NULL;
    
    // Добавляем всех детей в корневой список
    if (z->child != NULL) {
        FibNode *c = z->child;
        do {
            c->parent = NULL;
            c = c->right;
        } while (c != z->child);
        FibNode *last = z->child->left;
        z->child->left = heap->min->left;
        heap->min->left->right = z->child;
        last->right = heap->min;
        heap->min->left = last;
    }
    
    fib_heap_remove_from_root_list(heap, z);
    if (z == z->right) {
        heap->min = NULL;
    } else {
        heap->min = z->right;
        fib_heap_consolidate(heap);
    }
    heap->n--;
    return z;
}

// Проверка на пустоту
int fib_heap_empty(FibHeap *heap) {
    return heap->min == NULL;
}