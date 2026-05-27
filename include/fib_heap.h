#ifndef FIB_HEAP_H
#define FIB_HEAP_H

// Узел фибоначчиевой кучи
typedef struct FibNode {
    int vertex;                // номер вершины
    long long key;             // ключ (вес ребра)
    struct FibNode *parent;    // родитель
    struct FibNode *child;     // ребёнок
    struct FibNode *left;      // левый брат
    struct FibNode *right;     // правый брат
    int degree;                // количество детей
    int mark;                  // метка (был ли удалён ребёнок)
} FibNode;

// Структура фибоначчиевой кучи
typedef struct FibHeap {
    FibNode *min;              // указатель на минимальный узел
    int n;                     // количество узлов
} FibHeap;

// Создаёт пустую фибоначчиеву кучу
FibHeap* fib_heap_create(void);

// Освобождает память кучи
void fib_heap_free(FibHeap *heap);

// Вставляет вершину с ключом в кучу
void fib_heap_insert(FibHeap *heap, int vertex, long long key);

// Извлекает узел с минимальным ключом
FibNode* fib_heap_extract_min(FibHeap *heap);

// Проверяет, пуста ли куча
int fib_heap_empty(FibHeap *heap);

#endif