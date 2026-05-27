#include "fib_heap.h"
#include <stdlib.h>
#include <stdio.h>

FibHeap* fib_heap_create(void) {
    FibHeap *h = (FibHeap*)malloc(sizeof(FibHeap));
    h->min = NULL;
    h->n = 0;
    return h;
}

void fib_heap_free(FibHeap *h) {
    // В учебном проекте можно не удалять все узлы (это долго)
    free(h);
}

static void fib_heap_add_to_root_list(FibHeap *h, FibNode *node) {
    if (h->min == NULL) {
        node->left = node;
        node->right = node;
        h->min = node;
    } else {
        node->left = h->min;
        node->right = h->min->right;
        h->min->right->left = node;
        h->min->right = node;
    }
}

static void fib_heap_remove_from_root_list(FibHeap *h, FibNode *node) {
    if (node == h->min && node->right == node) {
        h->min = NULL;
    } else {
        node->left->right = node->right;
        node->right->left = node->left;
        if (node == h->min) h->min = node->right;
    }
    node->left = node->right = node;
}

static void fib_heap_link(FibHeap *h, FibNode *y, FibNode *x) {
    fib_heap_remove_from_root_list(h, y);
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

static void fib_heap_consolidate(FibHeap *h) {
    int D = 64;
    FibNode **A = (FibNode**)calloc(D, sizeof(FibNode*));
    FibNode *w = h->min;
    do {
        FibNode *x = w;
        int d = x->degree;
        while (A[d] != NULL) {
            FibNode *y = A[d];
            if (x->key > y->key) {
                FibNode *t = x;
                x = y;
                y = t;
            }
            fib_heap_link(h, y, x);
            A[d] = NULL;
            d++;
        }
        A[d] = x;
        w = w->right;
    } while (w != h->min);
    
    h->min = NULL;
    for (int i = 0; i < D; i++) {
        if (A[i] != NULL) {
            if (h->min == NULL || A[i]->key < h->min->key)
                h->min = A[i];
            fib_heap_add_to_root_list(h, A[i]);
        }
    }
    free(A);
}

void fib_heap_insert(FibHeap *h, int vertex, long long key) {
    FibNode *node = (FibNode*)malloc(sizeof(FibNode));
    node->vertex = vertex;
    node->key = key;
    node->parent = NULL;
    node->child = NULL;
    node->left = node->right = node;
    node->degree = 0;
    node->mark = 0;
    fib_heap_add_to_root_list(h, node);
    if (h->min == NULL || key < h->min->key)
        h->min = node;
    h->n++;
}

FibNode* fib_heap_extract_min(FibHeap *h) {
    FibNode *z = h->min;
    if (z == NULL) return NULL;
    
    // Добавляем всех детей в корневой список
    if (z->child != NULL) {
        FibNode *c = z->child;
        do {
            c->parent = NULL;
            c = c->right;
        } while (c != z->child);
        // Объединяем детей с корневым списком
        FibNode *last = z->child->left;
        z->child->left = h->min->left;
        h->min->left->right = z->child;
        last->right = h->min;
        h->min->left = last;
    }
    
    fib_heap_remove_from_root_list(h, z);
    if (z == z->right) {
        h->min = NULL;
    } else {
        h->min = z->right;
        fib_heap_consolidate(h);
    }
    h->n--;
    return z;
}

int fib_heap_empty(FibHeap *h) {
    return h->min == NULL;
}