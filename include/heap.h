#ifndef HEAP_H
#define HEAP_H

#include <stddef.h>

// Узел бинарной кучи
typedef struct HeapNode {
    int vertex;        // номер вершины
    long long key;     // ключ (вес ребра)
} HeapNode;

// Структура бинарной кучи (приоритетная очередь)
typedef struct Heap {
    HeapNode *data;    // массив узлов
    int *pos;          // позиция вершины в куче (для decrease-key)
    int size;          // текущий размер
    int cap;           // вместимость
} Heap;

// Создаёт кучу на max_vertices вершин
Heap* heap_create(int max_vertices);

// Освобождает память кучи
void heap_free(Heap *h);

// Проверяет, пуста ли куча
int heap_empty(Heap *h);

// Добавляет вершину с ключом в кучу
void heap_push(Heap *h, int v, long long key);

// Уменьшает ключ вершины (делает его меньше)
void heap_decrease_key(Heap *h, int v, long long new_key);

// Извлекает вершину с минимальным ключом
HeapNode heap_pop(Heap *h);

#endif